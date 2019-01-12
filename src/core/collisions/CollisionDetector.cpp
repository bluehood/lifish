#include "CollisionDetector.hpp"

using lif::CollisionDetector;

CollisionDetector::CollisionDetector(lif::EntityGroup& group, const sf::FloatRect& limit)
	: group(group)
	, levelLimit(limit)
{}
