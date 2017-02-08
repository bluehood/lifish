#pragma once

#include "Sighted.hpp"
#include "Direction.hpp"

namespace lif {

/** An AxisSighted entity has knowledge of entities around it along axis directions. */
class AxisSighted : public lif::Sighted {
	/** Pair <entity, distance in tiles> */
	using SeenPair = std::pair<std::weak_ptr<lif::Entity>, unsigned>;
	using PartSeenEntitiesList = std::vector<SeenPair>;
	using TotSeenEntitiesList = std::array<
		PartSeenEntitiesList,
		static_cast<unsigned short>(lif::Direction::NONE)>;

	/** All entities seen in each direction, and their distance (nearest first).
	 *  This array is indexed by Directions, treated as unsigned.
	 */
	TotSeenEntitiesList seen;
	std::array<float, static_cast<unsigned short>(lif::Direction::NONE)> vision;

	/** Fills seen[dir] with entities seen in that direction */
	void _fillLine(const lif::Direction dir);

public:
	explicit AxisSighted(lif::Entity& owner, float visionRadius = -1);

	/** @return A list of pair {entity, distance_in_tiles (discrete)} as seen in all directions */
	const TotSeenEntitiesList& entitiesSeen() const { return seen; }

	/** @return A list of pair {entity, distance_in_tiles (discrete)} as seen along direction `dir` */
	const PartSeenEntitiesList& entitiesSeen(lif::Direction dir) const {
		return seen[static_cast<std::size_t>(dir)];
	}
	
	/** @return The number of pixels seen by entity before first opaque entity along `dir`;
	 *  if none, return -1.
	 */
	float getVision(lif::Direction dir) const {
		return vision[static_cast<std::size_t>(dir)];
	}

	lif::Entity* init() override;
	void update() override;
};

}
