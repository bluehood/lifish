#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector2.inl>
#include <string>

#include "Entity.hpp"

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

/**
 * A sf::Text container which draws a string with a shadow behind
 */
class ShadedText final : public lif::Entity, public sf::Drawable {
	sf::Text fgtext, bgtext;
	sf::Vector2f shadowSpacing;

public:
	ShadedText() {}
	ShadedText(
		const std::string& fontname,
		const std::string& str,
		sf::Vector2f pos = sf::Vector2f(0, 0),
		sf::Color fgcol = sf::Color::White,
		sf::Color bgcol = sf::Color::Black);

	sf::FloatRect getGlobalBounds() const;
	std::string getString() const;

	void setStyle(sf::Text::Style style);
	void setString(const std::string& str);
	void setCharacterSize(unsigned int size);
	void setShadowSpacing(float spx, float spy);
	void setFGColor(const sf::Color& fg) { fgtext.setFillColor(fg); }
	void setBGColor(const sf::Color& bg) { bgtext.setFillColor(bg); }
	void setColor(const sf::Color& fg, const sf::Color& bg);
	void setFont(const std::string& fontname);
	void setScale(float x, float y);

	void setPosition(const sf::Vector2f& pos) override;
	void setOrigin(const sf::Vector2f& origin) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}
