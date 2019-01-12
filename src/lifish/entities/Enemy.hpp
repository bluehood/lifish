#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

#include "Attack.hpp"
#include "Entity.hpp"
#include "game.hpp"

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

class AI;
class Bonusable;
class Enemy;
class Collider;
class Animated;
class Killable;
class MovingAnimator;
class AlienSprite;
class AxisSighted;
class AxisMoving;
class Shooting;
class AutoShooting;
class RegularEntityDeath;

struct EnemyInfo {
	unsigned short ai = 0;
	unsigned short speed = 0;
	Attack attack;
};

/**
 * This class provides a Drawable proxy to draw the regular enemy's sprite
 * or its AlienSprite according to the morphed state of the Enemy.
 */
class EnemyDrawableProxy : public sf::Drawable {
	const lif::Enemy& enemy;
	const lif::Animated& morphedAnim;

public:
	explicit EnemyDrawableProxy(const lif::Enemy& e);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

/**
 * An Enemy is a MovingEntity which (usually) shoots towards players
 * when they see them and is vulnerable to Bombs.
 */
class Enemy : public lif::Entity {

	friend class EnemyDrawableProxy;

	std::unique_ptr<const lif::EnemyDrawableProxy> drawProxy;

	bool shootingAnim = false;

protected:
	constexpr static unsigned short WALK_N_FRAMES = 4;
	constexpr static int YELL_DELAY = 1000;

	const unsigned short id;
	const lif::EnemyInfo info;

	lif::AI *ai = nullptr;
	lif::Shooting *shooting = nullptr;
	lif::AutoShooting *autoShooting = nullptr;
	lif::Collider *collider = nullptr;
	lif::Animated *animated = nullptr;
	lif::AxisMoving *moving = nullptr;
	lif::Killable *killable = nullptr;
	lif::MovingAnimator *movingAnimator = nullptr;
	lif::AxisSighted *sighted = nullptr;
	lif::RegularEntityDeath *death = nullptr;
	lif::Bonusable *bonusable = nullptr;

	sf::Time morphDuration;

	/** True when the enemy is morphed into a harmless Alien during EXTRA game */
	bool morphed = false;
	lif::AlienSprite *alienSprite = nullptr;

	const float originalSpeed;

	/** Used to perform some actions before calling 'hurtByExplosions'.
	 *  @return true if 'hurtByExplosions' should NOT be called.
	 */
	virtual bool _checkCollision(lif::Collider& coll);
	void _checkShoot();
	/** @return Whether entity `e` is within our Shooting range or not */
	bool _inRange(const lif::Entity *const e) const;

public:
	explicit Enemy(const sf::Vector2f& pos, unsigned short id, const lif::EnemyInfo& info);

	void setMorphed(bool b);
	bool isMorphed() const { return morphed; }

	void update() override;
};

}
