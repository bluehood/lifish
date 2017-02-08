#include <exception>
#include "AutoShooting.hpp"
#include "Shooting.hpp"
#include "AxisBullet.hpp"
#include "FreeBullet.hpp"

using lif::AutoShooting;

AutoShooting::AutoShooting(lif::Entity& owner)
	: lif::BufferedSpawner(owner)
{}

lif::Entity* AutoShooting::init() {
	lif::BufferedSpawner::init();
	shooting = owner.get<Shooting>();
	if (shooting == nullptr)
		throw std::logic_error("owner of AutoShooting has no Shooting component!");
	owner.aliasComponent<lif::BufferedSpawner>(*this);
	return this;
}

void AutoShooting::shoot(lif::Direction dir) {
	addSpawned(shooting->shoot(dir));
}

void AutoShooting::shoot(lif::Angle angle) {
	addSpawned(shooting->shoot(angle));
}
