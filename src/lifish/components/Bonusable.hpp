#pragma once

#include <array>
#include <SFML/System/Time.hpp>
#include "Clock.hpp"
#include "Entity.hpp"
#include "BonusType.hpp"

namespace Game {

/** A component that can get bonuses */
class Bonusable : public Game::Component {
protected:
	// bonus time; sf::Time::Zero means "no bonus"; negative means 'infinite'
	std::array<sf::Time, Game::Conf::Bonus::N_BONUS_TYPES> bonusTime;
	std::array<Game::Clock*, Game::Conf::Bonus::N_BONUS_TYPES> bonusClock;

public:
	explicit Bonusable(Game::Entity& owner)
		: Game::Component(owner)
	{
		bonusClock.fill(addComponent(new Game::Clock(*this)));
		bonusTime.fill(sf::Time::Zero);
	}

	void giveBonus(Game::BonusType type, const sf::Time& time) {
		const auto i = static_cast<unsigned>(type);
		bonusTime[i] = time;
		bonusClock[i]->restart();
	}

	bool hasBonus(Game::BonusType type) const {
		const auto i = static_cast<unsigned>(type);
		return bonusTime[i] != sf::Time::Zero && bonusClock[i]->getElapsedTime() <= bonusTime[i];
	}

	sf::Time getTime(Game::BonusType type) const {
		return bonusTime[static_cast<unsigned>(type)]; 
	}

	sf::Time getElapsedTime(Game::BonusType type) const {
		return bonusClock[static_cast<unsigned>(type)]->getElapsedTime();
	}

	sf::Time getRemainingTime(Game::BonusType type) const {
		const auto i = static_cast<unsigned>(type);
		return std::max(sf::Time::Zero, bonusTime[i] - bonusClock[i]->getElapsedTime());
	}
	
	void reset() {
		bonusTime.fill(sf::Time::Zero);
	}

};

}
