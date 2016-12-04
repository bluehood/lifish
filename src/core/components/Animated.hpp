#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "AnimatedSprite.hpp"
#include "Animation.hpp"

namespace Game {

class Clock;

/**
 * An Animated is a drawable object whose sprite has a certain
 * number of associated animations.
 */
class Animated : public Game::Component, public sf::Drawable {
protected:
	sf::Texture *texture;
	Game::Clock *frameClock;
	std::unordered_map<std::string, Animation> animations;
	std::unordered_map<std::string, sf::Time> frameTimes;
	sf::Time defaultFrameTime = sf::Time::Zero;
	AnimatedSprite animatedSprite;
	bool manualPosition = false;

public:
	ENTITY_ID("Animated")
	COMP_NOT_UNIQUE

	explicit Animated(Game::Entity& owner, const std::string& texture_name);
	
	/** Adds a new empty animation to this Animated and returns it */
	Animation& addAnimation(const std::string& name);
	/** Adds a new animation tagged `name` with frames described by `frames`, and optionally sets it active. */
	Animation& addAnimation(const std::string& name, std::initializer_list<sf::IntRect> frames, bool set = false);

	/** If an animation tagged `name` exists, returns it. Else returns nullptr */
	Animation* getAnimation(const std::string& name);
	std::string getAnimationName() const;
	bool hasAnimation(const std::string& name) const;

	/** Sets the current animation to the one tagged `name`, if any.
	 *  Throws if `name` doesn't exist.
	 */
	void setAnimation(const std::string& name);
	/** Sets the current animation to `anim` */
	void setAnimation(Animation& anim);

	/** Sets the default frame time of all animations */
	void setDefaultFrameTime(sf::Time time);
	/** Sets the default frame time of animation `name` (overrides default frame time) */
	void setFrameTime(const std::string& name, sf::Time time);

	/** Returns true if cur animation == `name` and animatedSprite is playing */
	bool isPlaying(const std::string& name);

	sf::Texture* getTexture() const { return texture; }
	void setTexture(sf::Texture *t) { texture = t; }

	const AnimatedSprite& getSprite() const { return animatedSprite; }
	AnimatedSprite& getSprite() { return animatedSprite; }

	void update() override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setOrigin(const sf::Vector2f& o) override {
		animatedSprite.setOrigin(o);
	}

	void setPosition(const sf::Vector2f& pos) override {
		Game::Entity::setPosition(pos);
		manualPosition = true;
	}
};

}
