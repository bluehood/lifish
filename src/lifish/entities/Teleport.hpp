#pragma once

#include "Entity.hpp"

namespace Game {

class Animated;
class Clock;
class Collider;

/**
 * A teleport can be used both by players and enemies.
 */
class Teleport : public Game::Entity {
	static constexpr unsigned N_ANIM_FRAMES = 8;

	/** The Teleport this one teleports to */
	Teleport *_next = nullptr;

	bool disabled = false;
	/** Time to wait before reactivating */
	Game::Clock *disableClock = nullptr;
	Game::Animated *animated = nullptr;


	void warp(Game::Collider& cld);

public:
	explicit Teleport(const sf::Vector2f& pos);

	Teleport* next() const { return _next; }
	void linkTo(Teleport *nxt) { _next = nxt; }

	/** Sets disableCount to its max value, disabling the teleport
	 *  until it drops to 0.
	 */
	void disable();

	bool isDisabled() const { return disabled; }

	void update() override;
};

}
