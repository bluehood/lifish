#pragma once

#include <SFML/Graphics.hpp>
#include "Component.hpp"

namespace Game {

class Drawable : public Game::Component, public sf::Drawable {
	const sf::Drawable& delegate;

public:
	ENTITY_ID("Drawable")

	explicit Drawable(Game::Entity& owner, const sf::Drawable& delegate)
		: Game::Component(owner)
		, delegate(delegate) {}

	void draw(sf::RenderTarget& window, sf::RenderStates states) const override {
		window.draw(delegate, states);
	}
};

}
