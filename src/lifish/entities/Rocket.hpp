#pragma once

#include <SFML/System/Vector2.hpp>

#include "AxisBullet.hpp"
#include "Direction.hpp"

namespace lif {
class Entity;
struct BulletInfo;

class Rocket : public lif::AxisBullet {

	const float maxSpeed;

public:
	explicit Rocket(const sf::Vector2f& pos, lif::Direction dir, const lif::BulletInfo& info,
			const lif::Entity *const source = nullptr);

	void update() override;
};

}
