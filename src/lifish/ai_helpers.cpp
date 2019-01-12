#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector2.hpp>
#include <random>
#include <utility>

#include "ai_helpers.hpp"
#include "LevelManager.hpp"
#include "AxisMoving.hpp"
#include "AxisSighted.hpp"
#include "Entity.hpp"
#include "core.hpp"
#include "utils.hpp"

using D = lif::Direction;

namespace lif {

namespace ai {

std::array<lif::Direction, 4> directions = {{
	D::UP, D::RIGHT, D::DOWN, D::LEFT
}};

static std::uniform_int_distribution<> rand_dir_dist(0, directions.size());
lif::Direction randomDirection() {
	return directions[rand_dir_dist(lif::rng)];
}

lif::Direction selectRandomViable(
		const lif::AxisMoving& moving,
		const lif::LevelManager& lm,
		const lif::Direction opp,
		bool forceChange)
{
	lif::Direction dirs[directions.size()];
	unsigned short n = 0;
	for (const auto& d : directions)
		if (lm.canGo(moving, d) && d != opp && !(forceChange && moving.getDirection() == d))
			dirs[n++] = d;
	if (n == 0)
		dirs[n++] = opp;
	std::uniform_int_distribution<int> dist(0, n - 1);
	return dirs[dist(lif::rng)];
}

lif::Direction seeingPlayer(const lif::LevelManager& lm, const lif::AxisSighted& sighted) {
	const auto& seen = sighted.entitiesSeen();
	lif::Direction dir = lif::Direction::NONE;
	unsigned short dist = -1; // "infinity"
	for (unsigned i = 0; i < directions.size(); ++i) {
		for (const auto& pair : seen[i]) {
			if (lm.isPlayer(*pair.first) && pair.second < dist) {
				dir = static_cast<lif::Direction>(i);
				dist = pair.second;
			}
		}
	}
	return dir;
}

lif::Direction adjustPrevAlign(const lif::Entity& entity, lif::AxisMoving& moving) {
	switch (moving.getDirection()) {
	case D::LEFT:
	case D::UP:
		moving.setPrevAlign(lif::tile(entity.getPosition()));
		break;
	case D::RIGHT:
		moving.setPrevAlign(lif::tile(entity.getPosition()) + sf::Vector2i(1, 0));
		break;
	case D::DOWN:
		moving.setPrevAlign(lif::tile(entity.getPosition()) + sf::Vector2i(0, 1));
		break;
	case D::NONE:
		break;
	}
	moving.setDashing(0);

	return lif::oppositeDirection(moving.getDirection());
}

} // end namespace ai

} // end namespace lif
