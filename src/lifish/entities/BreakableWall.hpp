#pragma once

#include "Entity.hpp"

class Animation;

namespace Game {

class Killable;
class Collider;
class Animated;

/**
 * A breakable wall
 */
class BreakableWall : public Game::Entity {
	Game::Killable *killable = nullptr;
	Game::Animated *animated = nullptr;

	Game::Entity* _spawnBonus();
protected:
	/** Constructor used by TransparentWall */
	BreakableWall(const sf::Vector2f& pos, const std::string& texture_name, 
			unsigned life, unsigned score);

	Animation& _setupAnimations(const std::string& texture_name);
	void _setupComponents(unsigned life, unsigned score);
	void _checkCollision(Game::Collider& cld);

public:
	explicit BreakableWall(const sf::Vector2f& pos, const unsigned id, unsigned life = 1);
};

}
