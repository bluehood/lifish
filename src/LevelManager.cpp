#include "LevelManager.hpp"
#include "EntityType.hpp"
#include "FixedWall.hpp"
#include "BreakableWall.hpp"
#include "TransparentWall.hpp"
#include "Coin.hpp"
#include "Flash.hpp"
#include "BossExplosion.hpp"
#include "BossBullet.hpp"
#include "Enemy.hpp"
#include "Points.hpp"
#include "LevelLoader.hpp"
#include "utils.hpp"
#include <sstream>
#include <limits>

using Game::LevelManager;

LevelManager::LevelManager(std::initializer_list<Game::Player*> the_players)
	: renderer(this)
	, hurryUpText(Game::getAsset("test", "hurryup.png"), sf::Vector2i(161, 30), 300.f)
	, gameOverText(Game::getAsset("test", "gameover.png"), sf::Vector2i(311, 59))
	, extraGameText(Game::getAsset("test", "extragame.png"), sf::Vector2i(223, 156), 250.f)
{
	auto clock = addComponent(new Game::Clock<2>(this, { "bossShoot", "extraGame" }));
	bossShootClock = clock->getClock("bossShoot");
	extraGameClock = clock->getClock("extraGame");

	renderer = addComponent(new Game::LevelRenderer(this));

	static_assert(the_players.size() <= Game::MAX_PLAYERS, "Given more players to LevelManager than allowed!");
	unsigned short i = 0;
	for (auto& player : the_players) {
		_players[i++] = std::unique_ptr<Game::Player>(player);
		entities.add(player, false);
	}

	//movingEntities.reserve(LEVEL_WIDTH * LEVEL_HEIGHT);
	//temporary.reserve(LEVEL_WIDTH * LEVEL_HEIGHT);
	//bosses.reserve(32);
	//bullets.reserve(1024);
	//players.fill(nullptr);
}

LevelManager::~LevelManager() {
	_clearEntities();
	if (level != nullptr)
		delete level;
}

void LevelManager::_clearEntities() {
	// TODO

	//for (auto& e : fixedEntities)
		//if (e != nullptr) 
			//delete e;
	
	//for (auto& e : movingEntities)
		//if (!isPlayer(e))
			//delete e;
	
	//for (auto& b : bosses)
		//delete b;
	
	//for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i)
		//for (unsigned short j = 0; j < bombs[i].size(); ++j)
			//if (bombs[i][j] != nullptr) {
				//delete bombs[i][j];
				//bombs[i][j] = nullptr;
			//}

	//for (auto it = temporary.begin(); it != temporary.end(); ++it) 
		//delete *it;
	
	//for (auto it = explosions.begin(); it != explosions.end(); ++it) 
		//delete *it;
	
	//for (auto it = bullets.begin(); it != bullets.end(); ++it)
		//delete *it;
	
	//for (auto it = letters.begin(); it != letters.end(); ++it)
		//delete *it;

	//if (finalBoss != nullptr) {
		//finalBoss.reset();
	//}

	//fixedEntities.fill(nullptr);
	//players.fill(nullptr);
	//firstTeleport = nullptr;

	//temporary.clear();
	//explosions.clear();
	//movingEntities.clear();
	//bullets.clear();
	//bosses.clear();
	//letters.clear();
}

void LevelManager::loadLevel(Game::Level *const _level) {
	if (level != nullptr) {
		_clearEntities();
	}
	level = _level;
	bossShootClock->restart();
	hurryUp = false;
	if (levelTime != nullptr)
		delete levelTime;
	levelTime = addComponent(new Game::LevelTime(this, level->getInfo().time));

	//Game::Teleport *latest_teleport = nullptr;

	Game::LevelLoader::load(level, entities);
	
	if (movingEntities.size() > 0)
		letters.reserve(movingEntities.size() - 1);

	level->setOrigin(origin);
	for (auto& entity : fixedEntities) 
		if (entity != nullptr)
			entity->setOrigin(origin);
	for (auto& entity : movingEntities)
		entity->setOrigin(origin);
	for (auto& boss : bosses)
		boss->setOrigin(origin);

	if (finalBoss != nullptr)
		finalBoss->setOrigin(origin);

	for (const auto& player : players)
		if (player != nullptr)
			_pushTemporary(new Game::Flash(player->getPosition()));
}

void LevelManager::renderFrame(sf::RenderWindow& window) {
	if (level == nullptr) return;


}

void LevelManager::detectCollisions() {
	/* For each moving entity, check (towards its direction):
	 * 1) has it reached the level boundaries?
	 * 2) is there another non-trasparent entity occupying the cell ahead?
	 */
	std::vector<bool> checked(movingEntities.size(), false);
	
	auto collide = [] (const sf::Vector2f& pos, const sf::Vector2f& opos, const Game::Direction dir) {
		const unsigned short iposx = (unsigned short)pos.x,
			             iposy = (unsigned short)pos.y,
			             ioposx = (unsigned short)opos.x,
			             ioposy = (unsigned short)opos.y;
		switch (dir) {
		case Direction::UP:
			return iposx - TILE_SIZE < ioposx && ioposx < iposx + TILE_SIZE
				&& iposy >= ioposy && ioposy >= iposy - TILE_SIZE;
		case Direction::LEFT:
			return iposx >= ioposx && ioposx >= iposx - TILE_SIZE
				&& iposy - TILE_SIZE < ioposy && ioposy < iposy + TILE_SIZE;
		case Direction::DOWN:
			return iposx - TILE_SIZE < ioposx && ioposx < iposx + TILE_SIZE
				&& ioposy >= iposy && ioposy <= iposy + TILE_SIZE;
		case Direction::RIGHT:
			return ioposx >= iposx && ioposx <= iposx + TILE_SIZE
				&& iposy - TILE_SIZE < ioposy && ioposy < iposy + TILE_SIZE;
		default:
			return false;
		}
	};

	// Collision detection loop
	for (unsigned short i = 0, len = movingEntities.size(); i < len; ++i) {
		if (checked[i]) continue;
		MovingEntity *entity = movingEntities[i];
		const bool is_player = isPlayer(entity);
		if (!is_player && entity->isDying()) continue;
		sf::Vector2f pos = entity->getPosition();

		// Check for teleports
		if (firstTeleport != nullptr && entity->canTeleport && entity->isAligned()) {
			const auto cur_tile = Game::tile(pos);

			if (level->getTile(cur_tile.x - 1, cur_tile.y - 1) == EntityType::TELEPORT && entity->prevAlign != cur_tile) {
				const unsigned short idx = (cur_tile.y - 1) * LEVEL_WIDTH + cur_tile.x - 1;

				// Get Teleport from fixed entities
				auto teleport = static_cast<Game::Teleport*>(fixedEntities[idx]);

				if (teleport != nullptr && !teleport->isDisabled()) {
					// Get destination Teleport
					Game::Teleport *next = teleport->next();
					if (next == nullptr) continue;
					if (next->isDisabled() || isEntityTouching(next->getPosition())) {
						Game::Teleport *self = next;
						bool viable = false;
						do {
							next = next->next();
							if (next == teleport || next->isDisabled() || isEntityTouching(next->getPosition()))
								next = next->next();
							else {
								viable = true;
								break;
							}
						} while (next != self);
						// Check if we've found an enabled destination
						if (!viable) continue;
					}

					_pushTemporary(new Game::Flash(teleport->getPosition()));
					_pushTemporary(new Game::Flash(next->getPosition()));

					// Teleport the entity
					entity->setPosition(next->getPosition());
					entity->prevAlign = Game::tile(next->getPosition());

					// Disable both source and destination for a while
					teleport->disable();
					next->disable();
				}
			}
		}
		
		// Check for letters
		if (extraGame && is_player) {
			for (auto it = letters.begin(); it != letters.end(); ) {
				auto letter = *it;
				sf::FloatRect letter_box(letter->getPosition().x, letter->getPosition().y,
						TILE_SIZE, TILE_SIZE);
				sf::FloatRect player_box(pos.x, pos.y, TILE_SIZE, TILE_SIZE);
				if (player_box.intersects(letter_box)) {
					// Grab the letter
					auto player = static_cast<Game::Player*>(entity);
					player->extra[letter->getId()] = true;
					Game::cache.playSound(letter->getSoundFile());
					_spawnPoints(letter->getPosition(), letter->getPointsGiven());

					// Check if EXTRA
					bool extra = true;
					for (unsigned short i = 0; i < player->extra.size(); ++i) {
						if (!player->extra[i]) {
							extra = false;
							break;
						}
					}
					if (extra) {
						player->extra.fill(false);
						player->setRemainingLives(player->getRemainingLives() + 1);
						const auto upText = new Game::Points(letter->getPosition() 
								+ sf::Vector2f((TILE_SIZE - 45) / 2., 0.f),
								Game::to_string(_getPlayerIndex(player) + 1) + "UP",
								sf::Color(77, 184, 255, 255), 15);
						_pushTemporary(upText);
						Game::cache.playSound(Game::getAsset("test", Game::EXTRA_LIFE_SOUND));
					}

					delete letter;
					it = letters.erase(it);
				} else {
					++it;
				}
			}
		}

		// Check impact with bullets
		if (!entity->transparentTo.bullets) {
			for (auto it = bullets.begin(); it != bullets.end(); ++it) {
				auto bullet = *it;
				if (entity != bullet->getSource() && bullet->hits(entity->getPosition())) {
					if (is_player && !(entity->hasShield() || entity->isDying())) {
						// Damage player
						auto player = static_cast<Game::Player*>(entity);
						player->decLife(bullet->getDamage());
						spawnDamage(player->getPosition(), bullet->getDamage());
						if (!player->isHurt()) {
							player->setHurt(true);
							player->giveShield(Game::DAMAGE_SHIELD_TIME);
							Game::cache.playSound(player->getSoundFile(Game::Sounds::HURT));
						}
						if (player->getLife() <= 0) {
							player->kill();
							Game::cache.playSound(player->getSoundFile(Game::Sounds::DEATH));
						}
					} 

					if (!bullet->isBeingDestroyed()) {
						Game::cache.playSound(bullet->getSoundFile(Game::Sounds::DEATH));
						bullet->destroy();	
					}
				}
			}
		}

		entity->colliding = false;

		pos = entity->getPosition();
		const Direction dir = entity->getDirection();
		const unsigned short iposx = (unsigned short)pos.x,
		                     iposy = (unsigned short)pos.y;
		sf::Vector2i next_tile(iposx / TILE_SIZE - 1, iposy / TILE_SIZE - 1);

		if (entity->isOverBoundaries(dir)) {
			entity->colliding = true;
			continue;
		}

		const bool at_limit = entity->isAtLimit(dir);
		
		// Check for impacts with the borders
		switch (dir) {
		case Direction::UP:
			--next_tile.y;
			break;
		case Direction::LEFT:
			--next_tile.x;
			break;
		case Direction::DOWN:
			++next_tile.y;
			break;
		case Direction::RIGHT:
			++next_tile.x;
			break;
		case Direction::NONE:
			break;
		}

		// Check for impacts with moving entities
		bool collision_detected = false;
		for (unsigned short j = 0; j < len; ++j) {
			if (i == j) continue;

			MovingEntity *other = movingEntities[j];
			sf::Vector2f opos = other->getPosition();

			if (collide(pos, opos, dir)) {
				checked[i] = true;

				bool opaque;
				Game::Player *player = nullptr;
				Game::Enemy *enemy = nullptr;
				if (!is_player) {
					// If opaque == false, other entity is a player.
					opaque = !other->transparentTo.enemies;
					if (opaque) {
						collision_detected = true;
						entity->colliding = true;
					} else {
						player = static_cast<Game::Player*>(other);
						enemy = static_cast<Game::Enemy*>(entity);
					}
				} else {
					// If opaque == false, other entity is an enemy.
					opaque = !other->transparentTo.players;
					if (opaque) {
						collision_detected = true;
						entity->colliding = true;
					} else {
						player = static_cast<Game::Player*>(entity);
						enemy = static_cast<Game::Enemy*>(other);
					}
				}

				if (!opaque && !(enemy->isDying() || player->isDying())) {
					if (!enemy->isMorphed() && (enemy->attack.type & Enemy::AttackType::CONTACT)) {
						if (!enemy->isShooting()) {
							enemy->shoot();
							if (!(enemy->attack.type & Enemy::AttackType::RANGED)) {
								// Play the sound only if the attack is not "dashing"
								// (see Enemy.hpp#AttackType)
								Game::cache.playSound(enemy->getSoundFile(Game::Sounds::ATTACK));
							}
						}
						enemy->attackAlign = Game::tile(player->getPosition());
					}

					if (!player->hasShield()) {
						if (enemy->attack.type & Enemy::AttackType::CONTACT) {
							player->decLife(enemy->attack.damage);
							spawnDamage(player->getPosition(), enemy->attack.damage);
						} else {
							player->decLife(1);
							spawnDamage(player->getPosition(), 1);
						}
						if (!player->isHurt()) {
							player->setHurt(true);
							player->giveShield(Game::DAMAGE_SHIELD_TIME);
							Game::cache.playSound(player->getSoundFile(Game::Sounds::HURT));
						}
						if (player->getLife() <= 0) {
							player->kill();
							Game::cache.playSound(player->getSoundFile(Game::Sounds::DEATH));
						}
					}
				}

				if (other->getDirection() == Game::oppositeDirection(dir)) {
					if (opaque)
						other->colliding = true;
					checked[j] = true;
				}

				break;
			}
		}

		// Check for impacts with fixed entities
		if (!collision_detected && at_limit) {
			const unsigned short idx = next_tile.y * LEVEL_WIDTH + next_tile.x;
			if (idx >= fixedEntities.size()) {
				// Should never happen: means we're outside the grid
				entity->colliding = true;
				continue;
			}
			Game::Entity *other = fixedEntities[idx];
			if (other != nullptr) {
				if ((is_player && !other->transparentTo.players) 
						|| (!is_player && !other->transparentTo.enemies))
				{
					// Colliding with a wall
					entity->colliding = true;
				} else {
					// Either a coin, a bonus or a teleport. We can know for sure
					// what this entity is with a single lookup to level->tiles:
					// if we find a teleport or a coin, it means it's that entity;
					// else, if we find a breakable, it's a bonus.
					switch (level->getTile(next_tile.x, next_tile.y)) {
					case EntityType::COIN:
						{
							// Grab the coin
							if (!is_player) break;
							auto coin = static_cast<Game::Coin*>(other);
							if (!coin->isBeingGrabbed()) {
								coin->grab();
								Game::cache.playSound(coin->getSoundFile());
								Game::score[_getPlayerIndex(entity)] += coin->getPointsGiven();
								_spawnPoints(coin->getPosition(), coin->getPointsGiven());
								if (--coinsNum == 0) {
									_triggerExtraGame();
								}
							}
							break;
						}
					case EntityType::BREAKABLE:
					case EntityType::TRANSPARENT_WALL:
						// Grab the bonus
						if (!is_player) break;
						_grabBonus(entity, static_cast<Game::Bonus*>(other), idx);
						break;
					default:
						break;
					}
				}
			}

			// Check for impacts with bosses
			for (const auto& boss : bosses) {
				if (!boss->isDying() && boss->occupies(next_tile + sf::Vector2i(1, 1))) {
					if (is_player) {
						if (!(entity->hasShield() || entity->isDying())) {
							entity->kill();
							break;
						}
					} else {
						entity->colliding = true;
						break;
					}
				}
			}

			if (finalBoss != nullptr && !finalBoss->isDying() 
					&& finalBoss->occupies(next_tile + sf::Vector2i(1, 1))) {
				if (is_player) {
					if (!(entity->hasShield() || entity->isDying())) {
						entity->kill();
						break;
					}
				} else {
					entity->colliding = true;
					break;
				}
			}
		}
	}

	// Check final boss vs walls
	if (finalBoss != nullptr)
		finalBoss->detectCollisions(this);
		

	// Check impact bullets - fixed/borders
	for (auto& bullet : bullets) {
		if (bullet->isBeingDestroyed()) continue;
		const auto pos = bullet->getPosition();
		const auto szpos = pos + sf::Vector2f(bullet->getSize(), bullet->getSize());
		if (szpos.x < TILE_SIZE || szpos.x >= (LEVEL_WIDTH + 1) * TILE_SIZE
				|| szpos.y < TILE_SIZE || szpos.y >= (LEVEL_HEIGHT + 1) * TILE_SIZE) {
			bullet->destroy();
			Game::cache.playSound(bullet->getSoundFile(Game::Sounds::DEATH));
			continue;
		}

		if (!bullet->isTransparentToWalls()) {
			const auto tile = Game::tile(pos);
			const unsigned short idx = (tile.y - 1) * LEVEL_WIDTH + tile.x - 1;
			if (idx >= fixedEntities.size()) {
				bullet->destroy();
				Game::cache.playSound(bullet->getSoundFile(Game::Sounds::DEATH));
				continue;
			}
			Game::Entity *other = fixedEntities[idx];
			if (other != nullptr && !other->transparentTo.bullets) {
				bullet->destroy();
				Game::cache.playSound(bullet->getSoundFile(Game::Sounds::DEATH));
				continue;
			}
		}
		
		for (const auto& boss : bosses) {
			if (!boss->isDying() && boss != bullet->getSource()
					&& boss->intersects(sf::FloatRect(szpos.x, szpos.y, 
							bullet->getSize(), bullet->getSize()))) {
				bullet->destroy();
				Game::cache.playSound(bullet->getSoundFile(Game::Sounds::DEATH));
				break;	
			}
		}
	}
}

void LevelManager::selectEnemyMoves() {
	if (finalBoss != nullptr) {
		finalBoss->chooseDirection(this);
	}

	for (auto& entity : movingEntities) {
		if (isPlayer(entity) || entity->isDying())
			continue;

		auto enemy = static_cast<Game::Enemy*>(entity);
		if (!enemy->isAligned()) {
			if (enemy->colliding) {
				// Fix prevAligns
				switch (enemy->getDirection()) {
				case Direction::LEFT: 
				case Direction::UP:
					enemy->prevAlign = Game::tile(enemy->getPosition());
					break;
				case Direction::RIGHT:
					enemy->prevAlign = Game::tile(enemy->getPosition()) + sf::Vector2i(1, 0);
					break;
				case Direction::DOWN:
					enemy->prevAlign = Game::tile(enemy->getPosition()) + sf::Vector2i(0, 1);
					break;
				case Direction::NONE:
					break;
				}
				enemy->setDashing(false);
				enemy->setDirection(oppositeDirection(entity->getDirection()));
			}
		} else if (!enemy->isBlocked()) {
			enemy->setDirection(enemy->getAI()(this));
		}
	}
}

void LevelManager::moveBullets() {
	for (auto& bullet : bullets) {
		bullet->move();
	}
}

void LevelManager::applyEnemyMoves() {
	if (finalBoss != nullptr && !finalBoss->isDying()) {
		if (finalBoss->isAligned())
			finalBoss->prevAlign = Game::tile(finalBoss->getPosition());
		finalBoss->move();
	}

	for (auto& entity : movingEntities) {
		if (isPlayer(entity) || entity->isDying()) 
			continue;

		if (entity->isAligned()) {
			const auto cur_align = Game::tile(entity->getPosition());
			entity->prevAlign = cur_align; 
		}

		auto enemy = static_cast<Game::Enemy*>(entity);

		// Check if this enemy should shoot
		if (!extraGame
				&& !(enemy->attack.type & Game::Enemy::AttackType::CONTACT)
				&& enemy->seeingPlayer == enemy->getDirection()) 
		{
			if (!enemy->isRecharging()) { 
				if (!(enemy->attack.type & Game::Enemy::AttackType::RANGED)
						|| enemy->distanceWithNearestPlayer <= enemy->attack.range)
				{
					enemy->shoot();
					auto bullet = new Game::Bullet(
								enemy->getPosition(), 
								enemy->getDirection(),
								enemy->attack.id,
								enemy->attack.speed,
								enemy->attack.damage,
								enemy->attack.range);
					bullet->setOrigin(origin);
					bullet->setSource(enemy);
					bullets.push_back(bullet);
					Game::cache.playSound(bullet->getSoundFile(Game::Sounds::SHOT));

					if (enemy->attack.type & Game::Enemy::AttackType::BLOCKING) {
						if (enemy->attack.type & Game::Enemy::AttackType::SIMPLE)
							enemy->block();
						else {
							enemy->stop();
							continue;
						}
					}
				}
			} else if (enemy->attack.type & Game::Enemy::AttackType::BLOCKING
					&& !(enemy->attack.type & Game::Enemy::AttackType::SIMPLE)) {
				enemy->stop();
				continue;
			}
		}
		entity->move();
	}
}

bool LevelManager::isEntityTouching(const sf::Vector2f& tile) const {
	sf::FloatRect tileRect(tile.x, tile.y, TILE_SIZE, TILE_SIZE);
	for (auto& entity : movingEntities) {
		sf::Vector2f pos = entity->getPosition();
		sf::FloatRect rect(pos.x, pos.y, TILE_SIZE, TILE_SIZE);
		if (rect.intersects(tileRect)) return true;
	}
	return false;
}

void LevelManager::_pushTemporary(Game::Temporary *const tmp) {
	temporary.push_back(tmp);
	tmp->setOrigin(origin);
	tmp->play();
}

void LevelManager::dropBomb(const unsigned short id) {
	// Count how many bombs the player has already dropped
	unsigned short n_bombs = 0, idx = 0;
	const auto pl_tile = Game::tile(players[id]->getPosition());
	for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i) {
		for (unsigned short j = 0; j < Game::Player::MAX_MAX_BOMBS; ++j) {
			if (bombs[i][j] == nullptr) {
				if (i == id) 
					idx = j;
			} else {
				// Don't drop 2 bombs in the same tile
				if (Game::tile(bombs[i][j]->getPosition()) == pl_tile)
					return;
				if (i == id) 
					++n_bombs;
			}
		}
	}
	if (n_bombs == players[id]->powers.maxBombs) return;
	auto bomb = new Game::Bomb(
			players[id]->getPosition(),
			players[id],
			players[id]->powers.bombFuseTime,
			players[id]->powers.bombRadius);
	Game::cache.playSound(bomb->getSoundFile());
	bombs[id][idx] = bomb;
	bomb->setOrigin(origin);

}

void LevelManager::checkBombExplosions() {
	for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i)
		for (unsigned short j = 0; j < bombs[i].size(); ++j)
			if (bombs[i][j] != nullptr && bombs[i][j]->isExploding()) {
				auto expl = new Game::Explosion(
						bombs[i][j]->getPosition(), 
						bombs[i][j]->getRadius(), 
						bombs[i][j]->getSourcePlayer());
				Game::cache.playSound(expl->getSoundFile());
				expl->propagate(this);
				expl->setOrigin(origin);
				expl->play();
				explosions.push_back(expl);
				bombs[i][j]->blowUp();
				expl->checkHit(this);
			}
}

void LevelManager::checkExplosionHits() {
	for (auto& expl : explosions)
		expl->checkHit(this);
}

Game::Bomb* LevelManager::getBombAt(const unsigned short left, const unsigned short top) const {
	for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i)
		for (unsigned short j = 0; j < bombs[i].size(); ++j) {
			if (bombs[i][j] == nullptr) continue;
			const auto tile = Game::tile(bombs[i][j]->getPosition());
			if (tile.x == left && tile.y == top) return bombs[i][j];
		}
	return nullptr;
}

void LevelManager::checkLinesOfSight() {
	std::array<sf::Vector2i, Game::MAX_PLAYERS> pos;
	for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i)
		if (players[i] != nullptr)
			pos[i] = Game::tile(players[i]->getPosition());

	for (auto& e : movingEntities) {
		if (isPlayer(e)) continue;
		auto enemy = static_cast<Game::Enemy*>(e);
		if (enemy->isDying() || enemy->isRecharging()) continue;

		enemy->seeingPlayer = Direction::NONE;
		const auto epos = Game::tile(e->getPosition());
		// set a distance greater than any possible one
		enemy->distanceWithNearestPlayer = 2 * LEVEL_WIDTH; 
		for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i) {
			if (players[i] == nullptr || players[i]->isDying()) continue;
			if (pos[i].x == epos.x) {
				const short dist = _getDistance(epos, pos[i], false);
				// _getDistance returns -1 if sight is blocked
				if (dist < 0) break;
				if (dist < enemy->distanceWithNearestPlayer) {
					enemy->seeingPlayer = epos.y < pos[i].y 
								? Direction::DOWN 
								: Direction::UP;
					enemy->distanceWithNearestPlayer = dist;
				}
			} else if (pos[i].y == epos.y) {
				const short dist = _getDistance(epos, pos[i], true);
				if (dist < 0) break;
				if (dist < enemy->distanceWithNearestPlayer) {
					enemy->seeingPlayer = epos.x < pos[i].x 
								? Direction::RIGHT 
								: Direction::LEFT;
					enemy->distanceWithNearestPlayer = dist;
				}
			}
		}
	}
}

short LevelManager::_getDistance(const sf::Vector2i& src, const sf::Vector2i& target, bool axis_x) {
	unsigned short start, end;
	// Assume src and target are on the same axis and:
	//   if no opaque entity is between them, return their distance
	//   else return -1.
	if (axis_x) {
		if (src.x < target.x)
			start = src.x, end = target.x;
		else
			start = target.x, end = src.x;

		for (unsigned short i = start + 1; i < end; ++i) {
			const unsigned short idx = (src.y - 1) * LEVEL_WIDTH + i - 1;
			if (idx >= fixedEntities.size()) 
				return -1;

			auto fxd = fixedEntities[idx];
			if (fxd != nullptr && !fxd->transparentTo.bullets)
				return -1;
		}
	} else {
		if (src.y < target.y)
			start = src.y, end = target.y;
		else
			start = target.y, end = src.y;

		for (unsigned short i = start + 1; i < end; ++i) {
			const unsigned short idx = (i - 1) * LEVEL_WIDTH + src.x - 1;
			if (idx >= fixedEntities.size()) 
				return -1;
			
			auto fxd = fixedEntities[idx];
			if (fxd != nullptr && !fxd->transparentTo.bullets)
				return -1; 
		}
	}
	return end - start;
}

bool LevelManager::isPlayer(const Game::Entity *const e) const {
	for (unsigned short i = 0; i < players.size(); ++i)
		if (e == players[i]) return true;
	return false;
}

bool LevelManager::removePlayer(const unsigned short id, bool overrideInternal) {
	movingEntities.erase(std::remove(movingEntities.begin(), 
				movingEntities.end(), players[id-1]), movingEntities.end());

	players[id-1] = nullptr;
	if (overrideInternal)
		_players[id-1].reset();

	for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i)
		if (players[i] != nullptr) return true;

	return false;
}

void LevelManager::setPlayer(const unsigned short id, Game::Player *player) {
	_players[id-1].reset(player);
	players[id-1] = _players[id-1].get();
}

short LevelManager::_getPlayerIndex(const Game::Entity *const e) const {
	for (unsigned short i = 0; i < Game::MAX_PLAYERS; ++i)
		if (e == players[i]) return i;
	return -1;
}

void LevelManager::_spawnPoints(const sf::Vector2f& pos, const int amount, bool large) {
	// center the points in the tile
	auto points = large
			? new Game::Points(pos, Game::to_string(amount), sf::Color::Magenta, 20)
			: new Game::Points(pos, Game::to_string(amount));
	const auto bounds = points->getGlobalBounds();
	points->setPosition(points->getPosition() + sf::Vector2f((TILE_SIZE - bounds.width) / 2., 0.f));
	points->setOrigin(origin);
	_pushTemporary(points);
}

void LevelManager::spawnDamage(const sf::Vector2f& pos, const int amount) {
	auto dmg = new Game::Points(pos, Game::to_string(-amount));
	const auto bounds = dmg->getGlobalBounds();
	dmg->setColor(sf::Color::Red, sf::Color::White);
	dmg->setPosition(dmg->getPosition() + sf::Vector2f((TILE_SIZE - bounds.width) / 2., 0.f));
	dmg->setOrigin(origin);
	_pushTemporary(dmg);
}

void LevelManager::makeFinalBossShoot() {
	if (finalBoss == nullptr || finalBoss->isDying())
		return;

	if (finalBoss->shouldBreed()) {
		finalBoss->breed();
	}
}

void LevelManager::makeBossesShoot() {
	if (bosses.size() == 0)
		return;

	unsigned short shootInterval = Game::Boss::SHOOT_INTERVAL;

	if (bossClockCycle != 0)
		shootInterval = Game::Boss::SHOOT_SHORT_INTERVAL;
	else if (hurryUp) 
		shootInterval *= 0.6;

	if (bossShootClock.getElapsedTime().asMilliseconds() < shootInterval)
		return;

	if (++bossClockCycle == 3)
		bossClockCycle = 0;

	bossShootClock->restart();
	for (auto& boss : bosses) {
		if (boss->isDying()) continue;
		const auto shootingPts = boss->getShootingPoints();
		for (auto sp : shootingPts) {
			const auto ppos = _findNearestPlayer(sp);
			if (ppos.x < 0) {
				// no players found
				return;
			}
			if (Game::distance(sp, ppos) > Game::Boss::MAX_RANGE)
				continue;
		
			// calculate angle with ppos: a = pi - arctan(dy / dx)
			const double dx = sp.x - ppos.x,
			             dy = ppos.y - sp.y,
				     angle = Game::PI - std::atan2(dy, dx);

			auto bullet = new Game::BossBullet(sp, angle);
			bullet->setOrigin(origin);
			bullet->setSource(boss);
			bullets.push_back(bullet);
			Game::cache.playSound(bullet->getSoundFile(Game::Sounds::SHOT));
		}
	}
}

sf::Vector2f LevelManager::_findNearestPlayer(const sf::Vector2f& pos) const {
	sf::Vector2f nearest(-1.f, -1.f);

	for (const auto& player : players) {
		if (player == nullptr) continue;
		const auto ppos = player->getPosition();
		if (nearest.x < 0 || Game::distance(pos, ppos) < Game::distance(pos, nearest)) {
			nearest = ppos;
		}
	}
	return nearest;
}

void LevelManager::dropBonus(const sf::Vector2i& tile, const unsigned short type) {
	const unsigned short idx = (tile.y - 1) * Game::LEVEL_WIDTH + tile.x - 1;
	auto bonus = new Game::Bonus(sf::Vector2f(tile.x * TILE_SIZE, tile.y * TILE_SIZE), type);
	bonus->setOrigin(origin);
	fixedEntities[idx] = bonus;
}

void LevelManager::_destroyAllWalls() {
	for (auto& fxd : fixedEntities) {
		// Here it's acceptable to use dynamic_cast, since this function
		// is called very rarely (only when the Zapper is found)
		auto bw = dynamic_cast<Game::BreakableWall*>(fxd);
		if (bw != nullptr) {
			bw->destroy();
			Game::cache.playSound(bw->getSoundFile());
			for (unsigned short i = 0; i < players.size(); ++i)
				if (players[i] != nullptr && players[i]->getRemainingLives() > 0)
					Game::score[i] += bw->getPointsGiven();
			_spawnPoints(bw->getPosition(), bw->getPointsGiven());
		}
	}
}

void LevelManager::_killAllEnemies() {
	for (auto& e : movingEntities) {
		if (isPlayer(e)) continue;
		_pushTemporary(new Game::Explosion(e->getPosition(), 0));
		e->kill();
		auto se = dynamic_cast<Game::Scored*>(e);
		if (se != nullptr) {
			for (unsigned short i = 0; i < players.size(); ++i)
				if (players[i] != nullptr && players[i]->getRemainingLives() > 0)
					Game::score[i] += se->getPointsGiven();
			_spawnPoints(e->getPosition(), se->getPointsGiven());
		}
		Game::cache.playSound(e->getSoundFile(Game::Sounds::DEATH));

		if (extraGame)
			_spawnLetter(e->getPosition());
	}

	for (auto& b : bosses) {
		if (!b->isDying()) {
			_pushTemporary(new Game::Explosion(b->getPosition(), 0));
			b->decLife(1);
			b->hurt();
		}
	}
}

void LevelManager::checkHurryUp() {
	if (hurryUp) return;
	
	levelTime->update();

	switch (levelTime->isHurryUp()) {
	case LevelTime::HurryUpResponse::ON:
		// Trigger Hurry Up	
		for (auto& e : movingEntities) {
			if (isPlayer(e)) continue;
			auto enemy = static_cast<Game::Enemy*>(e);
			enemy->giveSpeedy(std::numeric_limits<int>::max());
			enemy->attack.fireRate *= 2.;		
		}
		hurryUp = true;
		break;
	case LevelTime::HurryUpResponse::HURRY_UP_NEAR:
		hurryUpText.play();
		hurryUpText.setOrigin(origin);
		Game::cache.playSound(Game::getAsset("test", Game::HURRY_UP_SOUND));
		break;
	default:
		break;
	}
}

void LevelManager::triggerGameOver() {
	Game::stopMusic();
	gameOverText.play(); 
	gameOverText.setOrigin(origin); 
	Game::cache.playSound(Game::getAsset("test", Game::GAME_OVER_SOUND));
}

void LevelManager::_triggerExtraGame() {
	extraGameText.play(); 
	extraGameText.setOrigin(origin);
	Game::cache.playSound(Game::getAsset("test", Game::EXTRA_GAME_SOUND));
	for (auto& e : movingEntities) {
		if (isPlayer(e)) continue;
		auto enemy = static_cast<Game::Enemy*>(e);
		enemy->setMorphed(true);
	}
	extraGame = true;
	extraGameClock->restart();
}

void LevelManager::checkExtraGameEnd() {
	if (!extraGame) return;
	if (extraGameClock.getElapsedTime().asSeconds() > 30) {
		extraGame = false;

		// Delete all letters
		for (auto it = letters.begin(); it != letters.end(); ) {
			delete *it;
			it = letters.erase(it);
		}

		// Change back enemies
		for (auto& e : movingEntities) {
			if (isPlayer(e)) continue;
			auto enemy = static_cast<Game::Enemy*>(e);
			enemy->setMorphed(false);
		}
	}
}

void LevelManager::_spawnLetter(const sf::Vector2f& pos) {
	auto letter = new Game::Letter(pos, Game::Letter::randomId());
	letter->setOrigin(origin);
	letters.push_back(letter);
}

void LevelManager::cycleLetters() {
	for (auto& letter : letters)
		letter->checkTransition();
}

void LevelManager::givePointsTo(const Game::Player *const player, 
		const sf::Vector2f& pos, const int amount) {
	if (player == nullptr) return;
	_spawnPoints(pos, amount);
	Game::score[_getPlayerIndex(player)] += amount;
}

bool LevelManager::isLevelClear() const {
	if (bosses.size() > 0) return false;
	for (const auto& m : movingEntities)
		if (!isPlayer(m))
			return false;
	if (finalBoss != nullptr) return false;
	return true;
}

void LevelManager::resetFrameClocks() {
	for (auto& e : movingEntities) {
		e->resetClock();
	}
	if (finalBoss != nullptr)
		finalBoss->resetClock();
}
	
void LevelManager::resetClocks() {
	bossShootClock->restart();
	levelTime->reset();
	resetFrameClocks();
}

void LevelManager::_grabBonus(Game::MovingEntity *const entity, 
		Game::Bonus *bonus, unsigned short idx) {
	
	const auto i = _getPlayerIndex(entity);

	switch (bonus->getType()) {

		using B = Game::Bonus::Type;

	case B::ZAPPER:
		_destroyAllWalls();
		break;
	case B::SUDDEN_DEATH:
		_killAllEnemies();
		break;
	case B::MAX_BOMBS:
		if (players[i]->powers.maxBombs < Game::Player::MAX_MAX_BOMBS)
			players[i]->powers.maxBombs += 1;
		break;
	case B::QUICK_FUSE:
		if (players[i]->powers.bombFuseTime == Game::Bomb::DEFAULT_FUSE) {
			players[i]->powers.bombFuseTime /= 2.;
			for (auto& b : bombs[i])
				if (b != nullptr && !b->isIgnited() && !b->isExploding())
					b->setFuseTime(players[i]->powers.bombFuseTime);
		}
		break;
	case B::MAX_RANGE:
		if (players[i]->powers.bombRadius < Game::Bomb::MAX_RADIUS) {
			players[i]->powers.bombRadius += 1;
			for (auto& b : bombs[i])
				if (b != nullptr && !b->isIgnited() && !b->isExploding())
					b->setRadius(players[i]->powers.bombRadius);
		}
		break;
	case B::HEALTH_SMALL:
		if (players[i]->getLife() < Game::Player::MAX_LIFE)
			players[i]->decLife(-2);
		break;
	case B::HEALTH_FULL:
		if (players[i]->getLife() < Game::Player::MAX_LIFE)
			players[i]->setLife(players[i]->getMaxLife());
		break;
	case B::SHIELD:
		players[i]->giveShield(Game::Bonus::SHIELD_DURATION);
		break;
	case B::SPEEDY:
		players[i]->giveSpeedy(Game::Bonus::SPEEDY_DURATION);
		break;
	default:
		std::cerr << "[ LevelManager ] Warning: unknown bonus id "
			<< bonus->getType() << std::endl;
	}

	Game::score[i] += bonus->getPointsGiven();
	Game::cache.playSound(bonus->getSoundFile());
	_spawnPoints(bonus->getPosition(), bonus->getPointsGiven());

	delete bonus;
	fixedEntities[idx] = nullptr;
}

void LevelManager::pauseClocks() {
	// Pause our own clocks
	pauseClock();

	for (auto& entity : fixedEntities) {
		auto ce = dynamic_cast<Game::Clocked*>(entity);
		if (ce != nullptr)
			ce->pauseClock();
	}
	for (auto& entity : movingEntities)
		entity->pauseClock();

	for (auto& boss : bosses)
		boss->pauseClock();

	if (finalBoss != nullptr)
		finalBoss->pauseClock();

	for (auto& tmp : temporary)
		tmp->pauseClock();

	for (auto& expl : explosions)
		expl->pauseClock();

	for (unsigned short i = 0; i < bombs.size(); ++i)
		for (auto& bomb : bombs[i])
			if (bomb != nullptr)
				bomb->pauseClock();

	for (auto& bullet : bullets)
		bullet->pauseClock();

	gameOverText.Clocked::pauseClock();
	extraGameText.Clocked::pauseClock();
	hurryUpText.Clocked::pauseClock();
}

void LevelManager::resumeClocks() {
	resumeClock();

	for (auto& entity : fixedEntities) {
		auto ce = dynamic_cast<Game::Clocked*>(entity);
		if (ce != nullptr)
			ce->resumeClock();
	}
	for (auto& entity : movingEntities)
		entity->resumeClock();

	for (auto& boss : bosses)
		boss->resumeClock();

	if (finalBoss != nullptr)
		finalBoss->resumeClock();

	for (auto& tmp : temporary)
		tmp->resumeClock();

	for (auto& expl : explosions)
		expl->resumeClock();

	for (unsigned short i = 0; i < bombs.size(); ++i)
		for (auto& bomb : bombs[i])
			if (bomb != nullptr)
				bomb->resumeClock();

	for (auto& bullet : bullets)
		bullet->resumeClock();

	gameOverText.resumeClock();
	extraGameText.resumeClock();
	hurryUpText.resumeClock();
}

bool LevelManager::_isFinalLevel() const {
	return level != nullptr && level->getLevelNum() == level->getLevelSet()->getLevelsNum();
}