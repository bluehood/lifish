#pragma once

#include <SFML/System.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"

namespace lif {

class Grabbable;
class Animated;

/** An EXTRA letter. */
class Letter : public lif::Entity {
	const static sf::Time TRANSITION_DELAY;

	/** What letter is this (currently) */
	unsigned short id;

	/** The clock driving the letter's transitions */
	lif::Grabbable *grabbable = nullptr;
	lif::Animated *animated = nullptr;

	sf::Time transitionT = sf::Time::Zero;

	/** Whether this letter is transitioning or not */
	bool transitioning = false;

public:
	static unsigned short randomId();

	explicit Letter(const sf::Vector2f& pos, unsigned short id);

	unsigned short getId() const { return id; }

	void update() override;
};

}
