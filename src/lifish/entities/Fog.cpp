#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector2.inl>
#include <random>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Fog.hpp"
#include "Drawable.hpp"
#include "FreeMoving.hpp"
#include "Sprite.hpp"
#include "Time.hpp"
#include "ZIndexed.hpp"
#include "conf/zindex.hpp"
#include "game.hpp"
#include "Entity.inl"
#include "core.hpp"

using lif::Fog;

Fog::Fog(float speed, sf::Time alphaDt)
	: lif::Entity(sf::Vector2f(0, 0))
	, alphaDt(alphaDt)
{
	sprite = addComponent<lif::Sprite>(*this, lif::getAsset("graphics", "fog.png"),
				sf::IntRect(0, 0,
					3 * lif::GAME_WIDTH,
					3 * lif::GAME_HEIGHT));
	sprite->getTexture()->setRepeated(true);
	sprite->getTexture()->setSmooth(true);
	addComponent<lif::Drawable>(*this, *sprite);
	addComponent<lif::ZIndexed>(*this, lif::conf::zindex::FOG);

	// Set a random velocity
	std::uniform_real_distribution<float> dist(-1, 1);
	const sf::Vector2f velocity(dist(lif::rng), dist(lif::rng));
	moving = addComponent<lif::FreeMoving>(*this, speed, velocity);

	// Set the initial position
	position.x = lif::TILE_SIZE - lif::GAME_WIDTH;
	position.y = lif::TILE_SIZE - lif::GAME_HEIGHT;
	sprite->getSprite().setColor(sf::Color(255, 255, 255, 255));
}

void Fog::update() {
	lif::Entity::update();

	t += lif::time.getDelta();

	// Fade in/out sprite
	if (t >= alphaDt) {
		t = sf::Time::Zero;
		auto c = sprite->getSprite().getColor();
		if (fading) {
			c.a -= 1;
			if (c.a < 5) {
				fading = false;
				// Reset the position
				position.x = lif::TILE_SIZE - lif::GAME_WIDTH;
				position.y = lif::TILE_SIZE - lif::GAME_HEIGHT;
			}
		} else {
			c.a += 1;
			if (c.a > 200)
				fading = true;
		}
		sprite->getSprite().setColor(c);
	}
}
