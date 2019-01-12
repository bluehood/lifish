#pragma once

#include <SFML/System/Vector2.hpp>

#include "GuidedBullet.hpp"
#include "Angle.hpp"

namespace lif {

class Drawable;
class Entity;
struct BulletInfo;

class Missile : public lif::GuidedBullet {

	lif::Angle rotRate;

	lif::Drawable *drawable = nullptr;

public:
	explicit Missile(const sf::Vector2f& pos, const sf::Vector2f& target,
		const lif::BulletInfo& info, const lif::Entity *const source = nullptr);

	void update() override;
};

}
