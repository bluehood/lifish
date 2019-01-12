#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"

namespace lif {

class Animated;
class Collider;

/**
 * A teleport can be used both by players and enemies.
 */
class Teleport : public lif::Entity {
	static constexpr unsigned int N_ANIM_FRAMES = 8;

	/** The Teleport this one teleports to */
	Teleport *_next = nullptr;

	bool disabled = false;
	/** Used by Spawning to spawn flashes */
	bool mustSpawnFlash = false;

	lif::Collider *collider = nullptr;
	lif::Animated *animated = nullptr;

	/** Time to wait before reactivating */
	sf::Time disableT = sf::Time::Zero;

	void _warp(lif::Collider& cld);

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
