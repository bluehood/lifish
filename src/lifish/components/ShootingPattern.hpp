#pragma once

#include "Attack.hpp"
#include "BufferedSpawner.hpp"

namespace lif {

/**
 * This is the base abstract class for a component that performs some kind of shooting pattern with a given Attack.
 * This component starts deactivated by default (since it shoots whenever it's active).
 */
class ShootingPattern : public lif::BufferedSpawner {
protected:
	lif::BulletInfo bullet;

	virtual void _reset() {}
public:
	COMP_NOT_UNIQUE

	explicit ShootingPattern(lif::Entity& owner, const lif::BulletInfo& bullet)
		: lif::BufferedSpawner(owner)
		, bullet(bullet)
	{
		active = false;
	}
	virtual ~ShootingPattern() {}

	void resetAndPlay() {
		_reset();
		setActive(true);
	}

	virtual lif::Entity* init() override {
		lif::BufferedSpawner::init();
		owner.aliasComponent<lif::BufferedSpawner>(*this);
		return this;
	}
	virtual void update() override = 0;
};

}
