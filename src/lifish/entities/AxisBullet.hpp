#pragma once

#include <SFML/System/Vector2.hpp>

#include "Bullet.hpp"
#include "Direction.hpp"

namespace lif {
class Entity;

/** A Bullet which travels along axes */
class AxisBullet : public lif::Bullet {
public:
	/** Constructs a Bullet with a source Entity (using that Entity's position) */
	explicit AxisBullet(const sf::Vector2f& pos, lif::Direction dir, const lif::BulletInfo& info,
			const lif::Entity *const source = nullptr);
};

}
