#include "LevelNumText.hpp"
#include "game.hpp"
#include "utils.hpp"

using Game::LevelNumText;

LevelNumText::LevelNumText(Game::Entity& owner, const unsigned levelnum)
	: Game::Component(owner)
	, text(Game::getAsset("fonts", Game::Fonts::LEVELNUM),
		Game::to_string(levelnum),
		sf::Vector2f(Game::TILE_SIZE * (Game::LEVEL_WIDTH+1), 0))
{
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(20);
}

void LevelNumText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(text, states);
}

void LevelNumText::setOrigin(const sf::Vector2f& pos) {
	Game::Entity::setOrigin(pos);
	text.setOrigin(pos);
}
