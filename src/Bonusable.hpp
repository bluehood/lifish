#pragma once

#include <array>
#include "Clock.hpp"
#include "Bonus.hpp"
#include "Entity.hpp"

namespace Game {

/** A component that can get bonuses */
class Bonusable : public Game::Component {
protected:
	// bonus time; sf::Time::Zero means "no bonus"; negative means 'infinite'
	std::array<sf::Time, Game::Bonus::N_BONUS_TYPES> bonusTime;
	Game::Clock<Game::Bonus::N_BONUS_TYPES> *bonusClock;

public:
	explicit Bonusable(Game::Entity *const owner)
		: Game::Component(owner)
	{
		bonusClock = addComponent(new Game::Clock<Game::Bonus::N_BONUS_TYPES>(this));
		bonusTime.fill(sf::Time::Zero);
	}

	void giveBonus(Game::Bonus::Type type, const sf::Time& time) {
		const unsigned short i = static_cast<unsigned short>(type);
		bonusTime[i] = time;
		bonusClock->getClock(i)->restart();
	}

	bool hasBonus(Game::Bonus::Type type) const {
		const unsigned short i = static_cast<unsigned short>(type);
		return bonusTime[i] != sf::Time::Zero && bonusClock->getClock(i)->getElapsedTime() <= bonusTime[i];
	}

	void reset() {
		bonusTime.fill(sf::Time::Zero);
	}
};

}
