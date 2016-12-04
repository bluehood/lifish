#pragma once

#include <SFML/Graphics.hpp>
#include "Component.hpp"

namespace Game {

/**
 * A drawable non-animated sprite
 */
class Sprite : public Game::Component, public sf::Drawable {
protected:
	sf::Texture *texture;
	sf::Sprite sprite;

public:
	ENTITY_ID("Sprite")

	explicit Sprite(Game::Entity& owner, const std::string& texture_name);
	explicit Sprite(Game::Entity& owner, const std::string& texture_name,
			const sf::IntRect& textureDivision);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Texture* getTexture() const { return texture; }
	sf::Sprite& getSprite() { return sprite; }

	Game::Entity* init() override;
	void update() override;

	void setOrigin(const sf::Vector2f& o) override;
};

}
