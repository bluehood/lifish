#pragma once

#include <SFML/System.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Pond.hpp"

namespace lif {

/**
 * Fire is a Pond which only damages enemies
 */
class Fire : public lif::Pond {
public:
	/** Duration == sf::Time::Zero means 'infinite' */
	explicit Fire(const sf::Vector2f& pos, const sf::Vector2f& size, sf::Time duration = sf::Time::Zero);
};

}
