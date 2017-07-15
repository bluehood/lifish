#include "Boss.hpp"
#include "ZIndexed.hpp"
#include "game.hpp"
#include "Bonusable.hpp"
#include "Player.hpp"
#include "Drawable.hpp"
#include "Animated.hpp"
#include "Clock.hpp"
#include "BossExplosion.hpp"
#include "Spawning.hpp"
#include "Collider.hpp"
#include "Killable.hpp"
#include "GameCache.hpp"
#include "Sounded.hpp"
#include "Explosion.hpp"
#include "HurtDrawProxy.hpp"
#include "Lifed.hpp"
#include "conf/zindex.hpp"
#include "conf/boss.hpp"
#include "conf/player.hpp"
#include "Foe.hpp"
#include "Absorbable.hpp"
#include <cassert>

using lif::Boss;
using lif::TILE_SIZE;

Boss::Boss(const sf::Vector2f& pos)
	: lif::Entity(pos)
{
	addComponent<lif::ZIndexed>(*this, lif::conf::zindex::BOSSES);
	addComponent<lif::Foe>(*this);
	explClock = addComponent<lif::Clock>(*this);
	deathClock = addComponent<lif::Clock>(*this);
	killable = addComponent<lif::Killable>(*this, [this] () {
		// on kill
		_kill();
	}, [this] () {
		// kill in progress
		return _killInProgress();
	});
	addComponent<lif::Spawning>(*this, [this] (const lif::Spawning&) {
		return killable && killable->isKilled()
			&& explClock->getElapsedTime() >= sf::milliseconds(100);
	}, [this] () {
		explClock->restart();
		// Calculate a random location inside the boss
		const auto bpos = collider->getPosition();
		std::uniform_real_distribution<float> distX(-0.5 * TILE_SIZE,
		                                            TILE_SIZE * (collider->getSize().x/TILE_SIZE - 0.5)),
		                                      distY(-0.5 * TILE_SIZE,
		                                            TILE_SIZE * (collider->getSize().y/TILE_SIZE - 0.5));
		const float x = distX(rng),
		            y = distY(rng);
		auto expl = new lif::BossExplosion(sf::Vector2f(bpos.x + x, bpos.y + y));
		lif::cache.playSound(expl->get<lif::Sounded>()->getSoundFile("explode"));
		return expl;
	});
	addComponent<lif::Drawable>(*this, *addComponent<lif::HurtDrawProxy>(*this));
	addComponent<lif::Absorbable>(*this);
}

lif::Entity* Boss::init() {
	lif::Entity::init();
	if (collider == nullptr)
		throw std::logic_error("Collider is null for " + toString() + "!");
	if (animated == nullptr)
		throw std::logic_error("Animated is null for " + toString() + "!");
	return this;
}

void Boss::_kill() {
	deathClock->restart();
	collider->setLayer(lif::c_layers::DEFAULT);
}

bool Boss::_killInProgress() const {
	return deathClock->getElapsedTime() < lif::conf::boss::DEATH_TIME;
}

void Boss::_checkCollision(lif::Collider& coll) {
	// One-shot players
	if (coll.getLayer() == lif::c_layers::PLAYERS) {
		auto& player = static_cast<lif::Player&>(coll.getOwnerRW());
		if (!player.get<lif::Bonusable>()->hasBonus(lif::BonusType::SHIELD)) {
			player.dealDamage(lif::conf::player::MAX_LIFE, true);
		}
		return;
	}

	if (coll.getLayer() != lif::c_layers::EXPLOSIONS) return;

	auto& expl = static_cast<lif::Explosion&>(coll.getOwnerRW());
	if (expl.hasDamaged(*this)) return;

	// Calculate how many explosion tiles overlap with boss's ones
	const auto brect = collider->getRect();
	const auto crect = coll.getRect();
	const short x = std::max(brect.left, crect.left),
	            wx = std::min(brect.left + brect.width, crect.left + crect.width),
	            y = std::max(brect.top, crect.top),
	            wy = std::min(brect.top + brect.height, crect.top + crect.height);
	
	assert(x > 0 && wx > 0 && y > 0 && wy > 0);

	const unsigned damage = std::round(static_cast<float>(wx - x) / lif::TILE_SIZE)
				* std::round(static_cast<float>(wy - y) / lif::TILE_SIZE) * expl.getDamage();
	
	if (get<lif::Lifed>()->decLife(damage) > 0)
		get<lif::HurtDrawProxy>()->hurt();
	else
		killable->kill();
	expl.dealDamageTo(*this);
	lif::cache.playSound(get<lif::Sounded>()->getSoundFile("hurt"));
}

void Boss::_addDefaultCollider(const sf::Vector2f& size) {
	collider = addComponent<lif::Collider>(*this, [this] (lif::Collider& coll) {
		// on collision
		_checkCollision(coll);
	}, lif::c_layers::BOSSES, size);
}
