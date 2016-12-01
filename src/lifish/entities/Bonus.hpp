#pragma once

#include <SFML/System.hpp>
#include "bonus_type.hpp"
#include "Entity.hpp"

namespace Game {

class Clock;
class Sprite;
class Grabbable;
class Player;

/**
 * The bonuses dropped by walls
 */
class Bonus : public Game::Entity {
	const Game::BonusType type;

	Game::Clock *expireClock = nullptr;
	Game::Sprite *sprite = nullptr;
	Game::Grabbable *grabbable = nullptr;

	void _grab(Game::Player& player);
public:
	ENTITY_ID("Bonus")

	explicit Bonus(const sf::Vector2f& pos, const Game::BonusType type);

	Game::BonusType getType() const { return type; }

	void update() override;
};

}
