#pragma once

#include <SFML/System.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"

namespace lif {

class Grabbable;
class Animated;

/**
 * A coin can be taken by any player. Taking all coins triggers
 * EXTRA game, which morphs all enemies into harmless Aliens.
 */
class Coin : public lif::Entity {

	sf::Time grabT;
	lif::Grabbable *grabbable = nullptr;

	lif::Animated *animated = nullptr;

	void _grab();

public:
	explicit Coin(const sf::Vector2f& pos);

	void update() override;
};

}
