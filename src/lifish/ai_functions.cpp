#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector2.hpp>
#include <random>
#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "ai_functions.hpp"
#include "ai_helpers.hpp"
#include "LevelManager.hpp"
#include "AxisMoving.hpp"
#include "GameCache.hpp"
#include "utils.hpp"
#include "AxisSighted.hpp"
#include "Shooting.hpp"
#include "Sounded.hpp"
#include "Collider.hpp"
#include "Direction.hpp"
#include "Entity.hpp"
#include "Entity.inl"
#include "core.hpp"

using lif::AIBoundFunction;
using D = lif::Direction;
using namespace lif::ai;

const std::array<lif::AIFunction, lif::AI_FUNCTIONS_NUM> lif::ai_functions = {{
	lif::ai_random,
	lif::ai_random_forward,
	lif::ai_random_forward_haunt,
	lif::ai_follow,
	lif::ai_follow_dash,
	lif::ai_chase,
}};

AIBoundFunction lif::ai_random(lif::Entity& entity) {
	auto moving = entity.get<lif::AxisMoving>();
	const auto collider = entity.get<lif::Collider>();
	if (moving == nullptr || collider == nullptr)
		throw std::invalid_argument("Entity passed to ai_random has no Moving or Collider component!");
	moving->setAutoRealign(false);

	return [&entity, moving, collider] (const lif::LevelManager& lm) {
		HANDLE_NOT_MOVING;
		HANDLE_UNALIGNED;
		if (!collider->isColliding()) {
			if (moving->getDistTravelled() < lif::TILE_SIZE)
				SAME_DIRECTION

			std::uniform_int_distribution<int> dist(0, 10);
			if (moving->getDistTravelled() < 2 * lif::TILE_SIZE && dist(lif::rng) <= 4)
				SAME_DIRECTION
		}
		D dirs[4];
		unsigned short n = 0;
		if (entity.isAligned('x')) {
			if (lm.canGo(*moving, D::UP)) dirs[n++] = D::UP;
			if (lm.canGo(*moving, D::DOWN)) dirs[n++] = D::DOWN;
		}
		if (entity.isAligned('y')) {
			if (lm.canGo(*moving, D::LEFT)) dirs[n++] = D::LEFT;
			if (lm.canGo(*moving, D::RIGHT)) dirs[n++] = D::RIGHT;
		}
		if (n < 1) {
			// If no direction is viable, choose a random one (and basically
			// just keep on bumping around until a direction is viable)
			std::uniform_int_distribution<int> d(0, 3);
			NEW_DIRECTION(directions[d(lif::rng)])
		} else {
			std::uniform_int_distribution<int> d(0, n - 1);
			NEW_DIRECTION(dirs[d(lif::rng)])
		}
	};
}

AIBoundFunction lif::ai_random_forward(lif::Entity& entity) {
	auto moving = entity.get<lif::AxisMoving>();
	const auto collider = entity.get<lif::Collider>();
	if (moving == nullptr || collider == nullptr)
		throw std::invalid_argument("Entity passed to ai_random_forward has no Moving"
				" or Collider component!");
	moving->setAutoRealign(false);
	moving->setDistTravelled(4);

	return [&entity, moving, collider] (const lif::LevelManager& lm) {
		HANDLE_NOT_MOVING;
		HANDLE_UNALIGNED;
		const D cur = moving->getDirection();
		const bool colliding = collider->collidesWithSolid();
		const D opp = lif::oppositeDirection(cur);
		// colliding with a moving entity
		if (colliding && lm.canGo(*moving, cur)) {
			NEW_DIRECTION(opp)
		}
		// Note: this `if` prevents the entity to change direction twice in a row even
		// when it shouldn't (due to the fact that, if its movement is less than 1px/frame,
		// it may be considered "aligned" again after changing direction.)
		if (moving->getDistTravelled() > 3 || moving->getDistTravelled() == 0) {
			collider->reset();
			NEW_DIRECTION(selectRandomViable(*moving, lm, opp))
		} else {
			SAME_DIRECTION
		}
	};
}

// Mean-O-Taur AI
AIBoundFunction lif::ai_random_forward_haunt(lif::Entity& entity) {
	auto moving = entity.get<lif::AxisMoving>();
	const auto collider = entity.get<lif::Collider>();
	auto shooting = entity.get<lif::Shooting>();
	if (moving == nullptr || collider == nullptr || shooting == nullptr)
		throw std::invalid_argument("Entity passed to ai_random_forward_haunt has no Moving, "
				"Shooting or Collider component!");
	moving->setAutoRealign(false);
	const auto random_forward = ai_random_forward(entity);

	return [&entity, moving, random_forward, collider, shooting] (const lif::LevelManager& lm) {
		HANDLE_NOT_MOVING;
		HANDLE_UNALIGNED;
		if (shooting->isShooting()) {
			const auto cur_align = lif::tile(entity.getPosition());
			const D cur = moving->getDirection();
			const bool colliding = collider->collidesWithSolid();
			if (colliding && lm.canGo(*moving, cur)) // colliding with another enemy or similar
				NEW_DIRECTION(lif::oppositeDirection(cur))

			if (shooting->getAttackAlign() == cur_align)
				NEW_DIRECTION(selectRandomViable(*moving, lm, lif::oppositeDirection(cur)))

			if (shooting->getAttackAlign().x < cur_align.x)
				NEW_DIRECTION(D::LEFT)
			else if (shooting->getAttackAlign().x > cur_align.x)
				NEW_DIRECTION(D::RIGHT)
			else if (shooting->getAttackAlign().y < cur_align.y)
				NEW_DIRECTION(D::UP)
			else
				NEW_DIRECTION(D::DOWN)

		} else if (shooting->getAttackAlign().x > 0) {
			D dir = D::DOWN;
			const auto cur_align = lif::tile(entity.getPosition());
			if (shooting->getAttackAlign().x < cur_align.x)
				dir =  D::LEFT;
			else if (shooting->getAttackAlign().x > cur_align.x)
				dir =  D::RIGHT;
			else if (shooting->getAttackAlign().y < cur_align.y)
				dir =  D::UP;
			shooting->setAttackAlign(sf::Vector2i(-1, shooting->getAttackAlign().y));
			NEW_DIRECTION(dir)
		}
		random_forward(lm);
	};
}

AIBoundFunction lif::ai_follow(lif::Entity& entity) {
	auto moving = entity.get<lif::AxisMoving>();
	const auto collider = entity.get<lif::Collider>();
	const auto sighted = entity.get<lif::AxisSighted>();
	if (moving == nullptr || collider == nullptr || sighted == nullptr)
		throw std::invalid_argument("Entity passed to ai_random_forward_haunt has no Moving"
				", Collider or AxisSighted component!");
	moving->setAutoRealign(false);
	moving->setDistTravelled(3);

	return [&entity, moving, collider, sighted] (const lif::LevelManager& lm) {
		HANDLE_NOT_MOVING;
		HANDLE_UNALIGNED;
		const D cur = moving->getDirection();
		const bool colliding = collider->collidesWithSolid();

		const D opp = lif::oppositeDirection(cur);
		if (colliding && lm.canGo(*moving, cur))
			NEW_DIRECTION(opp)

		auto sp = seeingPlayer(lm, *sighted);
		if (sp != lif::Direction::NONE) {
			auto sounded = entity.get<lif::Sounded>();
			if (sounded != nullptr)
				lif::cache.playSound(sounded->getSoundFile("yell"));
			NEW_DIRECTION(sp)
		}

		if (moving->getDistTravelled() > 1 || moving->getDistTravelled() == 0) {
			collider->reset();
			NEW_DIRECTION(selectRandomViable(*moving, lm, opp))
		} else {
			SAME_DIRECTION
		}
	};
}

AIBoundFunction lif::ai_follow_dash(lif::Entity& entity) {
	auto moving = entity.get<lif::AxisMoving>();
	const auto collider = entity.get<lif::Collider>();
	const auto sighted = entity.get<lif::AxisSighted>();
	const auto shooting = entity.get<lif::Shooting>();
	if (moving == nullptr || collider == nullptr || sighted == nullptr || shooting == nullptr)
		throw std::invalid_argument("Entity passed to ai_random_forward_haunt has "
				"no Moving, Collider, AxisSighted or Shooting component!");
	moving->setAutoRealign(false);
	moving->setDistTravelled(3);

	return [&entity, shooting, moving, collider, sighted] (const lif::LevelManager& lm) {
		HANDLE_NOT_MOVING;
		HANDLE_UNALIGNED;
		const D cur = moving->getDirection();
		const D opp = lif::oppositeDirection(cur);
		const bool colliding = collider->collidesWithSolid();

		// XXX: is this necessary?
		if (colliding) {
			moving->setDashing(0);
			if (lm.canGo(*moving, cur))
				NEW_DIRECTION(opp)
		}

		if (moving->isDashing())
			SAME_DIRECTION

		auto sp = seeingPlayer(lm, *sighted);
		if (sp != lif::Direction::NONE) {
			if (!shooting->isRecharging()) {
				shooting->shoot(moving->getOwner().getPosition());
				auto sounded = entity.get<lif::Sounded>();
				if (sounded != nullptr)
					lif::cache.playSound(sounded->getSoundFile("attack"));
			}
			NEW_DIRECTION(sp)
		}

		if (moving->getDistTravelled() > 1 || moving->getDistTravelled() == 0) {
			collider->reset();
			NEW_DIRECTION(selectRandomViable(*moving, lm, opp))
		} else {
			SAME_DIRECTION
		}
	};
}

AIBoundFunction lif::ai_chase(lif::Entity& entity) {
	// TODO
	return ai_follow(entity);
}
