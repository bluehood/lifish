#pragma once

#include "Component.hpp"
#include "Activable.hpp"
#include "Direction.hpp"

namespace Game {

class AxisMoving;
class Animated;

/** 
 * A Component which animates an Animated entity based on its direction.
 */
class MovingAnimator : public Game::Component, public Game::Activable {

	Game::AxisMoving *moving = nullptr;
	Game::Animated *animated = nullptr;
	Game::Direction prev = Game::Direction::NONE;

public:
	ENTITY_ID("MovingAnimator")

	explicit MovingAnimator(Game::Entity& owner);
	explicit MovingAnimator(Game::Entity& owner, Game::AxisMoving *m, Game::Animated *a);

	Game::Entity* init() override;
	void update() override;
};

}
