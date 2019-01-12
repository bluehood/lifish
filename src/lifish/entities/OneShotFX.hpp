#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <initializer_list>
#include <string>

#include "Entity.hpp"

namespace lif {

class Animated;

/** A OneShotFX is a temporary animated sprite which is spawned in the level by something
 *  and automatically self-destroys when its animation ends.
 */
class OneShotFX : public lif::Entity {
protected:
	lif::Animated *animated = nullptr;
public:
	explicit OneShotFX(const sf::Vector2f& pos, const std::string& texture,
			std::initializer_list<sf::IntRect> frames);
};

}
