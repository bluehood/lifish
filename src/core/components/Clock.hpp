#pragma once

#include <array>
#include "Chronometer.hpp"
#include "Component.hpp"

namespace Game {

class Clock : public Game::Component {
	sftools::Chronometer clock;

public:
	ENTITY_ID("Clock")
	COMP_NOT_UNIQUE

	explicit Clock(Game::Entity& owner)
		: Game::Component(owner)
	{}

	sf::Time restart() {
		return clock.restart();
	}

	sf::Time getElapsedTime() const {
		return clock.getElapsedTime();
	}

	sf::Time pause() {
		return clock.pause();
	}

	sf::Time resume() {
		return clock.resume();
	}
};

}
