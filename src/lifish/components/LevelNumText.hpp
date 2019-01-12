#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "Component.hpp"
#include "ShadedText.hpp"
#include "Entity.hpp"

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

class LevelNumText : public lif::Component, public sf::Drawable {
	sf::RectangleShape bg;
	lif::ShadedText text;

public:
	explicit LevelNumText(lif::Entity& owner, unsigned short levelnum);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setOrigin(const sf::Vector2f& pos) override;
};

}
