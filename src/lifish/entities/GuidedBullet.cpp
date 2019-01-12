#include <SFML/System/Time.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "GuidedBullet.hpp"
#include "Entity.inl"

namespace lif {
class Entity;
}  // namespace lif

using lif::GuidedBullet;

GuidedBullet::GuidedBullet(const sf::Vector2f& pos, const sf::Vector2f& end,
			const lif::BulletInfo& info, sf::Time timeTaken,
			const lif::Entity *const source,
			std::initializer_list<lif::GuidedMoving::ModFunc> funcs)
	: lif::Bullet(pos, info, source)
{
	moving = addComponent<lif::GuidedMoving>(*this, pos, end, timeTaken, funcs);
	_setup();
}
