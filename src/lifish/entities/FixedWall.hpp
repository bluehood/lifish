#pragma once

#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

#include "Entity.hpp"

namespace lif {

/**
 * An unbreakable wall
 */
class FixedWall : public lif::Entity {
public:
	explicit FixedWall(const sf::Vector2f& pos, const unsigned short id);
};

}
