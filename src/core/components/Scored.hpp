#pragma once

#include <exception>
#include "Component.hpp"

namespace Game {

/**
 * Something which is worth points
 */
class Scored : public Game::Component {
protected:
	const int pointsGiven;
	bool givenPoints = false;
	/** The id of the player who points are given to */
	int target = -1;

public:
	explicit Scored(Game::Entity& owner, const int points) 
		: Game::Component(owner)
		, pointsGiven(points)
	{}

	int getPointsGiven() const { return pointsGiven; }

	/** @return whether this entity already gave points to the player or not */
	bool hasGivenPoints() const { return givenPoints; }
	int givePoints() {
		givenPoints = true;
		return pointsGiven;
	}

	int getTarget() const { return target; }
	/** Sets this component to give points to `t`-th player */
	void setTarget(unsigned t);
};

}
