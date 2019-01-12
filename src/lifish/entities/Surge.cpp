#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.inl>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Surge.hpp"
#include "Animated.hpp"
#include "Bonusable.hpp"
#include "Drawable.hpp"
#include "Killable.hpp"
#include "Player.hpp"
#include "Temporary.hpp"
#include "Time.hpp"
#include "ZIndexed.hpp"
#include "conf/boss.hpp"
#include "conf/zindex.hpp"
#include "core.hpp"
#include "AnimatedSprite.hpp"
#include "Entity.inl"
#include "LevelManager.hpp"
#include "bonus_type.hpp"
#include "game.hpp"

using lif::Surge;

static const sf::Vector2f SIZE(1024, 64);

Surge::Surge(const sf::Vector2f& pos, const lif::LevelManager& lm, const lif::Angle& initialRotation,
		const lif::Angle& rotPerSecond, const lif::Angle& spannedAngle)
	: lif::Entity(pos)
	, lm(lm)
	, rotPerSecond(rotPerSecond)
{
	animated = addComponent<lif::Animated>(*this, lif::getAsset("graphics", "surge.png"));
	animated->setDefaultFrameTime(sf::seconds(0.07));
	animated->addAnimation("surge", {
		sf::IntRect(0, 0 * SIZE.y, SIZE.x, SIZE.y),
		sf::IntRect(0, 1 * SIZE.y, SIZE.x, SIZE.y),
		sf::IntRect(0, 2 * SIZE.y, SIZE.x, SIZE.y),
		sf::IntRect(0, 3 * SIZE.y, SIZE.x, SIZE.y),
		sf::IntRect(0, 4 * SIZE.y, SIZE.x, SIZE.y),
	}, true);
	animated->getSprite().setOrigin(SIZE.x * 0.5, SIZE.y * 0.5);
	animated->getSprite().setRotation(initialRotation.asDegrees());

	addComponent<lif::Drawable>(*this, *animated);
	addComponent<lif::ZIndexed>(*this, lif::conf::zindex::TALL_ENTITIES);
	addComponent<lif::Temporary>(*this, [this, initialRotation, spannedAngle] () -> bool {
		return std::abs(animated->getSprite().getRotation() - initialRotation.asDegrees()) >
			spannedAngle.asDegrees();
	});
}

void Surge::update() {
	lif::Entity::update();

	animated->getSprite().rotate(rotPerSecond.asDegrees() * lif::time.getDelta().asSeconds());

	_checkCollision();
}

void Surge::_checkCollision() {
	for (int i = 0; i < lif::MAX_PLAYERS; ++i) {
		auto player = lm.getPlayer(i + 1);
		if (player == nullptr || player->get<lif::Killable>()->isKilled()
				|| player->get<lif::Bonusable>()->hasBonus(lif::BonusType::SHIELD))
		{
			continue;
		}
		// rotate the point to the inverse angle than the Surge
		const auto diff = player->getPosition() - position;
		const auto angle = -animated->getSprite().getRotation();
		const float c = std::cos(lif::deg2rad(angle)),
		            s = std::sin(lif::deg2rad(angle));
		const auto rot = sf::Transform(c, -s,  0,
		                               s,  c,  0,
		                               0,  0,  0);
		const auto pos = position + rot.transformPoint(diff);
		const sf::FloatRect aabb(position.x - SIZE.x * 0.5 + lif::TILE_SIZE,
		                         position.y - SIZE.y * 0.5 + lif::TILE_SIZE,
		                         SIZE.x, SIZE.y);
		if (aabb.contains(pos)) {
			// deal damage
			player->dealDamage(lif::conf::boss::mainframe_boss::SURGE_DAMAGE);
		}
	}
}
