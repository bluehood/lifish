#include "LevelLoader.hpp"
#include "game.hpp"
#include "LevelSet.hpp"
#include "Player.hpp"
#include "Animated.hpp"
#include "Level.hpp"
#include "LevelManager.hpp"
#include "FixedWall.hpp"
#include "AxisSighted.hpp"
#include "Teleport.hpp"
#include "BreakableWall.hpp"
//#include "AlienBoss.hpp"
#include "AI.hpp"
#include "Flash.hpp"
#include "TransparentWall.hpp"
#include "Coin.hpp"
#include "Enemy.hpp"
#include "Lifed.hpp"
#include "Wisp.hpp"
#include "AlienPredator.hpp"
#include "AcidPond.hpp"
#include "LevelEffects.hpp"
#include "HauntedStatue.hpp"
#include "HauntingSpiritBoss.hpp"
#include "Spikes.hpp"
#include "Torch.hpp"
#include "Sprite.hpp"
#include "Cage.hpp"
#include "DroppingBomb.hpp"
#include "TimedLaser.hpp"
#include "LeapingMovement.hpp"
#include "RexBoss.hpp"
#include "GodEyeBoss.hpp"
#include "Flare.hpp"
#include <iostream>

using lif::TILE_SIZE;
using lif::LevelLoader;
using lif::EntityType;

bool LevelLoader::load(const lif::Level& level, lif::LevelManager& lm) {

	lm.levelTime->setTime(sf::seconds(level.getInfo().time));

	lif::Teleport *firstTeleport = nullptr,
	              *latestTeleport = nullptr;

	lm.reset();
	auto& entities = lm.getEntities();

	const auto lvinfo = level.getInfo();

	for (int top = 0; top < lvinfo.height; ++top) {
		for (int left = 0; left < lvinfo.width; ++left) {

			const sf::Vector2f curPos((left + 1) * TILE_SIZE, (top + 1) * TILE_SIZE);
			int enemy_id = 0;
			const auto& ls = level.getLevelSet();

			const auto is_game_over = [&lm] (auto id) -> bool {
				return lm.players[id] == nullptr || (
						lm.players[id]->getInfo().remainingLives <= 0
						&& lm.players[id]->template get<lif::Lifed>()->getLife() <= 0
						&& lm.getPlayerContinues(id + 1) <= 0
					);
			};

			auto add_player = [&lm, &entities, is_game_over] (int id, const sf::Vector2f& pos) {
				if (!is_game_over(id)) {
					lm.players[id]->setWinning(false);
					lm.players[id]->setPosition(pos);
					lm.players[id]->get<lif::Animated>()->setAnimation("idle");
					lm.players[id]->get<lif::Moving>()->stop();
					entities.add(lm.players[id]);
					entities.add(new lif::Flash(pos));
				}
			};

			switch (level.getTile(left, top)) {
			case EntityType::FIXED:
				entities.add(new lif::FixedWall(curPos, level.getInfo().tileIDs.fixed));
				break;

			case EntityType::BREAKABLE:
				entities.add(new lif::BreakableWall(curPos, level.getInfo().tileIDs.breakable));
				break;

			case EntityType::TRANSPARENT_WALL:
				entities.add(new lif::TransparentWall(curPos));
				break;

			case EntityType::ACID_POND:
				entities.add(new lif::AcidPond(curPos, sf::Vector2f(TILE_SIZE, TILE_SIZE)));
				break;

			case EntityType::COIN:
				entities.add(new lif::Coin(curPos));
				break;

			case EntityType::HAUNTED_STATUE:
				entities.add(new lif::HauntedStatue(curPos));
				break;

			case EntityType::PLAYER1:
				add_player(0, curPos);
				break;

			case EntityType::PLAYER2:
				add_player(1, curPos);
				break;

			case EntityType::SPIKES:
				entities.add(new lif::Spikes(curPos));
				break;

			case EntityType::TORCH:
				_addTorch(entities, curPos, level, left, top);
				break;

			case EntityType::TELEPORT:
				{
					auto teleport = new lif::Teleport(curPos);

					// Save the first Teleport added
					if (firstTeleport == nullptr)
						firstTeleport = teleport;
					else
						teleport->linkTo(firstTeleport);

					// If we had already added a Teleport, link it to this one.
					if (latestTeleport != nullptr)
						latestTeleport->linkTo(teleport);
					latestTeleport = teleport;

					entities.add(teleport);
					break;
				}

			//case EntityType::ALIEN_BOSS:
				//{
					//auto boss = new lif::AlienBoss(curPos);
					//for (auto s : boss->getAllRecursive<lif::Sighted>())
						//s->setEntityGroup(&lm.entities);
					//entities.add(boss);
				//}
				//break;

			case EntityType::HAUNTING_SPIRIT_BOSS:
				{
					auto boss = new lif::HauntingSpiritBoss(curPos);
					for (auto s : boss->getAllRecursive<lif::Sighted>())
						s->setEntityGroup(&lm.entities);
					entities.add(boss);
				}
				break;

			case EntityType::REX_BOSS:
				{
					auto boss = new lif::RexBoss(curPos);
					for (auto s : boss->getAllRecursive<lif::Sighted>())
						s->setEntityGroup(&lm.entities);
					boss->get<lif::AI>()->setLevelManager(&lm);
					entities.add(boss);
				}
				break;

			case EntityType::ENEMY1:
				enemy_id = 1;
				break;

			case EntityType::ENEMY2:
				enemy_id = 2;
				break;

			case EntityType::ENEMY3:
				enemy_id = 3;
				break;

			case EntityType::ENEMY4:
				enemy_id = 4;
				break;

			case EntityType::ENEMY5:
				enemy_id = 5;
				break;

			case EntityType::ENEMY6:
				enemy_id = 6;
				break;

			case EntityType::ENEMY7:
				enemy_id = 7;
				break;

			case EntityType::ENEMY8:
				enemy_id = 8;
				break;

			case EntityType::ENEMY9:
				enemy_id = 9;
				break;

			case EntityType::ENEMY10:
				enemy_id = 10;
				break;

			case EntityType::EMPTY:
				break;

			default:
				std::cerr << "Invalid tile at (" << left << ", " << top << "): "
					<< level.getTile(left, top) << std::endl;
				break;
			}

			if (enemy_id > 0) {
				lif::Enemy *enemy = nullptr;
				const auto& info = ls.getEnemyInfo(enemy_id);
				// Some enemies have their own classes, others are just 'Enemy'
				switch (enemy_id) {
				case 5:
					enemy = new lif::Wisp(curPos, info);
					break;
				case 10:
					enemy = new lif::AlienPredator(curPos, info);
					break;
				default:
					enemy = new lif::Enemy(curPos, enemy_id, info);
					break;
				}
				enemy->get<lif::AI>()->setLevelManager(&lm);
				auto sighted = enemy->get<lif::Sighted>();
				sighted->setEntityGroup(&lm.entities);
				sighted->setOpaque({ lif::c_layers::BREAKABLES, lif::c_layers::UNBREAKABLES });
				entities.add(enemy);
				// FIXME
				if (enemy_id == 4)
					enemy->addComponent<lif::LeapingMovement>(*enemy, sf::seconds(0.2))->init();
			}
		}
	}

	for (auto e : LevelEffects::getEffectEntities(level))
		entities.add(e);

	// FIXME
	if (level.getInfo().levelnum == 30) {
		auto eye = new lif::GodEyeBoss({ 13 * lif::TILE_SIZE, 6 * lif::TILE_SIZE }, lm);
		eye->get<lif::Sighted>()->setEntityGroup(&lm.entities);
		entities.add(eye);
		entities.add(new lif::Torch({ 0, 0 }, 5));
		auto torch = new lif::Torch({ (level.getInfo().width + 2) * lif::TILE_SIZE, 0 }, 5);
		torch->get<lif::Animated>()->getSprite().setScale(-1, 1);
		entities.add(torch);
		entities.add(new lif::Torch({ 0, (level.getInfo().height + 1) * lif::TILE_SIZE }, 5));
		torch = new lif::Torch(sf::Vector2f{ float((level.getInfo().width + 2) * lif::TILE_SIZE),
					float((level.getInfo().height + 1) * lif::TILE_SIZE) }, 5);
		torch->get<lif::Animated>()->getSprite().setScale(-1, 1);
		entities.add(torch);
	}

	if (level.hasEffect("darkness")) {
		// In case of darkness, we need the Players to have an AxisSighted component
		for (size_t i = 0; i < lm.players.size(); ++i) {
			if (lm.players[i] == nullptr) continue;
			auto sighted = lm.players[i]->addComponent<lif::AxisSighted>(*lm.players[i]);
			sighted->init();
			sighted->setEntityGroup(&lm.entities);
			sighted->setOpaque({
				lif::c_layers::BREAKABLES,
				lif::c_layers::UNBREAKABLES
			});
		}
		entities.add(new lif::Flare(sf::seconds(0.07f), sf::seconds(0.7f)));
	}

	lm.levelTime->resume();

	return true;
}

void LevelLoader::_addTorch(lif::EntityGroup& entities, const sf::Vector2f& curPos,
		const lif::Level& level, int left, int top)
{
	auto torch = new lif::Torch(curPos);
	
	// Tether the torch to a wall if possible
	
	// If it has a level limit nearby, tether to it
	if (left == 0)
		goto add_torch;
	else if (left == level.getInfo().width - 1)
		goto flip_and_add_torch;

	// If it has a wall on its left, tether to left
	switch (level.getTile(left - 1, top)) {
	case EntityType::BREAKABLE:
	case EntityType::FIXED:
	case EntityType::TRANSPARENT_WALL:
		goto add_torch;
	default:
		break;
	}

	// If it has a wall on its right, tether to right
	switch (level.getTile(left + 1, top)) {
	case EntityType::BREAKABLE:
	case EntityType::FIXED:
	case EntityType::TRANSPARENT_WALL:
		goto flip_and_add_torch;
	default:
		break;
	}

	// Else, give up and tether to whichever tile it has at its left
	goto add_torch;

flip_and_add_torch:
	torch->setPosition(torch->getPosition() + sf::Vector2f(lif::TILE_SIZE, 0));
	torch->get<lif::Animated>()->getSprite().setScale(-1, 1);
add_torch:
	entities.add(torch);
}
