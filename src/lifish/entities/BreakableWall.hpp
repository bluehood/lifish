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
			unsigned short life, unsigned int score);

	Animation& _setupAnimations(const std::string& texture_name);
	void _setupComponents(unsigned short life, unsigned int score);
	void _checkCollision(Game::Collider& cld);

public:
	ENTITY_ID("BreakableWall")

	explicit BreakableWall(const sf::Vector2f& pos, const unsigned short id, unsigned short life = 1);
};

}
