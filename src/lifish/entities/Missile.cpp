#include <math.h>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.inl>
#include <random>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Missile.hpp"
#include "Collider.hpp"
#include "Drawable.hpp"
#include "Killable.hpp"
#include "Spawning.hpp"
#include "Targeter.hpp"
#include "Temporary.hpp"
#include "Time.hpp"
#include "core.hpp"
#include "Bullet.hpp"
#include "Entity.hpp"
#include "Entity.inl"
#include "GuidedMoving.hpp"

using lif::Missile;

Missile::Missile(const sf::Vector2f& pos, const sf::Vector2f& target,
		const lif::BulletInfo& info, const lif::Entity *const source)
	: lif::GuidedBullet(pos, target, info, sf::seconds(1.0 / info.speed), source)
{
	collider->setActive(false); // collide with nothing, not even level bounds

	// randomize the wave function a bit
	std::uniform_real_distribution<float> dist(6, 20);
	const auto ampl = dist(lif::rng) * lif::TILE_SIZE;
	const auto freq = dist(lif::rng);
	const auto wave = [ampl, freq] (auto t) {
		const auto y = ampl * (t - t * t);
		const auto x = 0.5 * lif::TILE_SIZE * std::sin(freq * t * lif::PI);
		return sf::Vector2f(x, -y);
	};
	static_cast<lif::GuidedMoving*>(moving)->addModFunc(wave, false, false);

	// spawn/despawn the targeter
	addComponent<lif::Spawning>(*this, [] (const lif::Spawning& spw) {
		// spawn condition
		return spw.nSpawned() < 1;
	}, [this, target] () {
		// spawn function
		auto targeter = new lif::Targeter(target);
		auto klb = get<lif::Killable>();
		targeter->addComponent<lif::Temporary>(*targeter, [klb] () {
			// expire condition
			return klb->isKilled();
		});
		return targeter;
	});

	drawable = get<lif::Drawable>();
	drawable->setRotOrigin(data.size / 2, data.size / 2);
	rotRate = lif::degrees(180) / sf::seconds(1.0 / info.speed).asSeconds() * (target.x >= pos.x ? -1 : 1);
}

void Missile::update() {
	lif::Entity::update();

	if (static_cast<const lif::GuidedMoving*>(moving)->getPerc() >= 1) {
		collider->setActive(true);
		get<lif::Killable>()->kill();
	} else {
		drawable->setRotation(drawable->getRotation() + rotRate * lif::time.getDelta().asSeconds());
	}
}
