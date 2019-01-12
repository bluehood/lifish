#include <SFML/System/Vector2.inl>
#include <cmath>
#include <memory>
#include <unordered_map>
#include <vector>

#include "FreeBullet.hpp"
#include "FreeMoving.hpp"
#include "conf/bullet.hpp"
#include "core.hpp"
#include "Drawable.hpp"
#include "Angle.hpp"
#include "Entity.inl"

namespace lif {
class Entity;
}  // namespace lif

using lif::FreeBullet;
using lif::TILE_SIZE;

FreeBullet::FreeBullet(const sf::Vector2f& pos, lif::Angle angle,
		const lif::BulletInfo& info, const lif::Entity *const source)
	: lif::Bullet(pos, info, source)
{
	moving = addComponent<lif::FreeMoving>(*this, lif::conf::bullet::BASE_SPEED * info.speed,
				sf::Vector2f(std::sin(angle.asRadians()), -std::cos(angle.asRadians())));
	_setup();

	get<lif::Drawable>()->setRotOrigin(data.size / 2, data.size / 2);
	get<lif::Drawable>()->setRotation(lif::degrees(90) - angle);
}
