#pragma once

#include "Moving.hpp"

namespace Game {

/**
 * An object moving in any direction. Does not align to grid.
 */
class FreeMoving : public Game::Moving {
	/** Normalized velocity vector */
	sf::Vector2f velocity;

public:
	ENTITY_ID("FreeMoving")

	explicit FreeMoving(Game::Entity& owner, float speed, 
			sf::Vector2f velocity = sf::Vector2f(0.f, 0.f));

	sf::Vector2f getVelocity() const { return velocity; }
	void setVelocity(const sf::Vector2f& vel);

	void update() override;
};

}
