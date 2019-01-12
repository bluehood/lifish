#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <memory>
#include <functional>
#include <vector>

#include "Boss.hpp"
#include "game.hpp"
#include "state_function.hpp"

namespace sf {
class Color;
}  // namespace sf

namespace lif {

class Animated;
class BufferedSpawner;
class ShootingPattern;
class Killable;

class HauntingSpiritBoss : public lif::Boss {

	std::vector<std::weak_ptr<lif::Killable>> statues;
	std::weak_ptr<lif::Killable> targetStatue;
	/**
	 * 0. circle
	 * 1. spiral
	 * 2. scatter
	 */
	std::array<lif::ShootingPattern*, 3> shootPatterns;
	std::array<sf::Color, 3> shootColors;
	int curShootIdx = 0;
	bool selectedNewPattern = false;
	bool showedAtkCue = false;

	lif::ShootingPattern *curShootPattern = nullptr;
	lif::Animated *animated = nullptr;
	lif::BufferedSpawner *spawner = nullptr;

	sf::Time atkT = sf::Time::Zero,
	         animT = sf::Time::Zero,
	         hauntT = sf::Time::Zero;

	lif::ai::StateFunction stateFunction = std::bind(&HauntingSpiritBoss::_updateStart, this);

	lif::ai::StateFunction _updateStart();
	lif::ai::StateFunction _updateSearching();
	lif::ai::StateFunction _updateSelectNewStatue();
	lif::ai::StateFunction _updateTransitioningBegin();
	lif::ai::StateFunction _updateTransitioningEnd();
	lif::ai::StateFunction _updateHaunting();
	lif::ai::StateFunction _updateDying();

	bool _isShooting() const;
public:
	explicit HauntingSpiritBoss(const sf::Vector2f& pos);

	void update() override;
};

}
