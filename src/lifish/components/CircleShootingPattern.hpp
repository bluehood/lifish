#pragma once

#include <SFML/System.hpp>
#include "ShootingPattern.hpp"
#include "Angle.hpp"

namespace lif {

class Clock;

/**
 * Shoots bullets in a circular fashion:
 *        |   o
 *   o    |
 * o + o  |o  +  o
 *   o    |
 *        |   o
 */
class CircleShootingPattern : public lif::ShootingPattern {
	unsigned short shotsFired = 0;

	lif::Clock *shootClock = nullptr;

	void _shoot();
	void _reset() override;
public:
	sf::Time timeBetweenShots = sf::seconds(1);
	/** Self-deactivate after shooting this number of attacks. Negative means 'infinite'. */
	short consecutiveShots = -1;
	/** Number of bullets per shot */
	short bulletsPerShot = 1;
	/** Angle to rotate shoot axis after a shot. */
	lif::Angle rotationPerShot = lif::radians(0);
	/** Angle of the first bullet (relative to vertical axis, clockwise) */
	lif::Angle shootAngle = lif::radians(0);
	/** If true, `shootAngle` gets randomized at every reset */
	bool randomizeShootAngle = false;

	/** Note that, rather than passing all parameters in the constructor,
	 *  you just set it directly, as the useful variables are public for convenience.
	 */
	explicit CircleShootingPattern(lif::Entity& owner, const lif::BulletInfo& bullet);
	
	lif::Entity* init() override;
	void update() override;
};

}
