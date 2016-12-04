#pragma once

#include "Component.hpp"

namespace Game {

/**
 * A Fixed Entity is an entity which cannot ever move.
 * This enables some optimizations on this Component's owner, particularly for its lookup in
 * an EntityGroup (i.e. a tile can be checked for its presence in a random access fashion).
 */
class Fixed : public Game::Component {
public:
	ENTITY_ID("Fixed")

	explicit Fixed(Game::Entity& owner)
		: Game::Component(owner)
	{}
};

}
