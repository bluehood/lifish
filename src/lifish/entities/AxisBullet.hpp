#pragma once

#include "Bullet.hpp"
#include "Direction.hpp"

namespace Game {

class AxisBulletPresets;

/** A Bullet which travels along axes */
class AxisBullet : public Game::Bullet {
	friend class Game::AxisBulletPresets;

	/** If 1 => this bullet has the same animation for any direction, up to 8.
	 *  If 2 => this bullet has 2 different animations when traveling UP/DOWN or
	 *          LEFT/RIGHT, up to 4 per direction.
	 *  If 4 => this bullet has different animations for each direction, up to 2.
	 *  The effective number of frames is established by nMotionFrames.
	 */
	unsigned directionality = 1;

public:
	/** Constructs a Bullet with a source Entity (using that Entity's position) */
	explicit AxisBullet(const sf::Vector2f& pos, const Game::Entity *const source, 
			const Game::Direction dir, const Game::Attack& attack);
};

class AxisBulletPresets {
	friend class Game::AxisBullet;

	static void setup(Game::AxisBullet& b, unsigned id);
};

}
