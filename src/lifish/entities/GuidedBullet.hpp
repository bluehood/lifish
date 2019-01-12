#pragma once

#include <SFML/System/Vector2.hpp>
#include <initializer_list>

#include "Bullet.hpp"
#include "GuidedMoving.hpp"

namespace sf {
class Time;
}  // namespace sf

namespace lif {
class Entity;

/** A bullet with a GuidedMoving component. */
class GuidedBullet : public lif::Bullet {
public:
	explicit GuidedBullet(const sf::Vector2f& pos, const sf::Vector2f& end,
			const lif::BulletInfo& info, sf::Time timeTaken,
			const lif::Entity *const source = nullptr,
			std::initializer_list<lif::GuidedMoving::ModFunc> modfuncs = {});
};

}
