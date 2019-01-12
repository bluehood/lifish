#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

#include "Component.hpp"
#include "Entity.hpp"

namespace sf {
class RenderTarget;
class Texture;
}  // namespace sf

namespace lif {

/**
 * A drawable non-animated sprite
 */
class Sprite : public lif::Component, public sf::Drawable {
	bool manualPosition = false;

protected:
	sf::Texture *texture;
	sf::Sprite sprite;

public:
	COMP_NOT_UNIQUE

	explicit Sprite(lif::Entity& owner, const std::string& texture_name);
	explicit Sprite(lif::Entity& owner, const std::string& texture_name,
			const sf::IntRect& textureDivision);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Texture* getTexture() const { return texture; }
	sf::Sprite& getSprite() { return sprite; }

	lif::Entity* init() override;
	void update() override;

	void setOrigin(const sf::Vector2f& o) override;

	void setPosition(const sf::Vector2f& pos) override {
		lif::Component::setPosition(pos);
		manualPosition = true;
	}
};

}
