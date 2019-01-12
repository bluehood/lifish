#pragma once

#include <SFML/System/Vector2.hpp>

#include "GuidedBullet.hpp"

namespace lif {
class Entity;
struct BulletInfo;

class Grenade : public lif::GuidedBullet {
	sf::Vector2f _calculateEnd(const sf::Vector2f& pos,const sf::Vector2f& target, float range) const;
public:
	explicit Grenade(const sf::Vector2f& pos, const sf::Vector2f& target, const lif::BulletInfo& info,
			const lif::Entity *const source = nullptr);

	void update() override;
};

}
