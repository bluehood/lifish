#pragma once

#include <SFML/System.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"
#include "bonus_type.hpp"

namespace lif {

class Sprite;
class Grabbable;
class Player;

/**
 * The bonuses dropped by walls
 */
class Bonus : public lif::Entity {
	const lif::BonusType type;

	lif::Sprite *sprite = nullptr;
	lif::Grabbable *grabbable = nullptr;

	sf::Time expireT = sf::Time::Zero;

	void _grab(lif::Player& player);
public:
	explicit Bonus(const sf::Vector2f& pos, const lif::BonusType type);

	lif::BonusType getType() const { return type; }

	void update() override;
};

}
