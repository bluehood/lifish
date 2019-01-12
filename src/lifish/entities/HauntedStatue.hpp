#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"

namespace sf {
class Color;
class RenderTarget;
}  // namespace sf

namespace lif {

class Animated;
class HurtDrawProxy;

class HauntedStatue : public lif::Entity, public sf::Drawable {
	bool possessed = false;

	lif::Animated *animated = nullptr,
	              *spirit = nullptr;
	lif::HurtDrawProxy *hurtProxy = nullptr;

public:
	explicit HauntedStatue(const sf::Vector2f& pos);

	void setPossessed(bool b) { possessed = b; }
	bool isPossessed() const { return possessed; }

	void setSpiritColor(sf::Color c);

	void draw(sf::RenderTarget& window, sf::RenderStates states) const override;
};

}
