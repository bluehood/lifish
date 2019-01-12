#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.inl>

#include "LevelNumText.hpp"
#include "game.hpp"
#include "utils.hpp"
#include "core.hpp"

using lif::LevelNumText;

LevelNumText::LevelNumText(lif::Entity& owner, unsigned short levelnum)
	: lif::Component(owner)
	, bg(sf::Vector2f(lif::TILE_SIZE, lif::TILE_SIZE))
	, text(lif::getAsset("fonts", lif::fonts::LEVELNUM),
		lif::to_string(levelnum),
		sf::Vector2f(lif::GAME_WIDTH - lif::TILE_SIZE, 0))
{
	_declComponent<LevelNumText>();
	bg.setPosition(text.getPosition());
	bg.setFillColor(sf::Color(50, 50, 50, 200));
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(20);
	text.setPosition(text.getPosition() +
		lif::center(text.getGlobalBounds(), sf::FloatRect(0, 0, lif::TILE_SIZE, lif::TILE_SIZE)));
}

void LevelNumText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(bg, states);
	target.draw(text, states);
}

void LevelNumText::setOrigin(const sf::Vector2f& pos) {
	lif::Entity::setOrigin(pos);
	bg.setOrigin(pos);
	text.setOrigin(pos);
}
