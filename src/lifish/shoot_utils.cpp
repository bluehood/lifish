#include <memory>
#include <unordered_map>

#include "shoot_utils.hpp"
#include "FreeSighted.hpp"
#include "Player.hpp"
#include "Shooting.hpp"
#include "Entity.hpp"
#include "Entity.inl"
#include "utils.hpp"

void lif::shootToNearestPlayer(lif::Entity& shooter) {
	const auto player = shooter.get<lif::FreeSighted>()->nearest<lif::Player>();
	if (player == nullptr) return;

	const auto& ppos = player->getPosition();
	shooter.get<lif::Shooting>()->shoot(lif::angleBetween(shooter.getPosition(), ppos));
}
