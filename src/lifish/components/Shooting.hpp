#pragma once

#include "Component.hpp"
#include "Attack.hpp"
#include "Direction.hpp"
#include <exception>

namespace Game {

class Clock;
class AxisBullet;
class FreeBullet;
class AxisMoving;

class Shooting : public Game::Component {
	/** The duration of the shooting frame */
	const static sf::Time SHOOT_FRAME_TIME;
	
	bool manualPosition = false;

protected:
	/** Used by CONTACT attack AI */
	sf::Vector2i attackAlign;
	
	Attack attack;
	bool shooting = false;
	float fireRateMult = 1;
	sf::Vector2f offset;

	Game::Clock *rechargeClock = nullptr;
	Game::AxisMoving *ownerMoving = nullptr;
	
public:
	ENTITY_ID("Shooting")

	explicit Shooting(Game::Entity& owner, const Attack& attack);

	const Attack& getAttack() const { return attack; }
	
	sf::Vector2i getAttackAlign() const { return attackAlign; }
	void setAttackAlign(const sf::Vector2i& aa) { attackAlign = aa; }

	/** If attack is CONTACT and not RANGED, just reset the recharge clock and return nullptr.
	 *  If attack is also RANGED (i.e. "dashing"), also call setDashing(true) for the owner's
	 *  Moving component (throws if no Moving component is found.)
	 *  Else, create an AxisBullet described by `attack` and return it. 
	 *  The caller must take care of its destruction.
	 *  If dir is NONE, the bullet is shot in the direction of its owner. 
	 *  In this case, the owner must have an AxisMoving component, or an exception is thrown.
	 *  NOTE: this method does NOT check whether this entity is recharging.
	 */
	Game::AxisBullet* shoot(Game::Direction dir = Game::Direction::NONE);

	/** Creates a FreeBullet moving with angle `angle` (rad) from its owner.
	 *  Caller must take care of its destruction. 
	 *  Throws if attack is CONTACT.
	 *  NOTE: this method does NOT check whether this entity is recharging.
	 */
	Game::FreeBullet* shoot(double angle);
	bool isShooting() const { return shooting; }

	bool isRecharging() const;

	void setFireRateMult(float fr);
	float getFireRateMult() const { return fireRateMult; }

	void setPosition(const sf::Vector2f& pos) override;
	sf::Vector2f getPosition() const override;
	void setOffset(const sf::Vector2f& off) { offset = off; }

	Game::Entity* init() override;
	void update() override;
};

}
