#include "AlienBoss.hpp"
#include "Player.hpp"
#include "Collider.hpp"
#include "Sounded.hpp"
#include "Drawable.hpp"
#include "Lifed.hpp"
#include "Scored.hpp"
#include "Sprite.hpp"

using namespace Game::Conf::Boss::AlienBoss;
using Game::AlienBoss;
 
const sf::Vector2i AlienBoss::SIZE = sf::Vector2i(3 * Game::TILE_SIZE, 3 * Game::TILE_SIZE);

AlienBoss::AlienBoss(const sf::Vector2f& pos)
	: Game::Boss(pos)
{
	addComponent(new Game::Sounded(*this, {
		Game::getAsset("test", std::string("alienboss_death.ogg")),
		Game::getAsset("test", std::string("alienboss_hurt.ogg"))
	}));
	collider = addComponent(new Game::Collider(*this, [this] (Game::Collider& coll) {
		// on collision
		_checkCollision(coll);
	}, Game::Layers::BOSSES, SIZE));
	killable = addComponent(new Game::Killable(*this, [this] () {
		// on kill
		_kill();
	}, [this] () {
		// kill in progress
		// TODO
		return false;
	}));
	addComponent(new Game::Scored(*this, Game::Conf::Boss::AlienBoss::VALUE));
	addComponent(new Game::Drawable(*this, *addComponent(new Game::Sprite(*this, 
				Game::getAsset("test", "alien_boss.png"), sf::IntRect(0, 0, SIZE.x, SIZE.y)))));
	addComponent(new Game::Lifed(*this, Game::Conf::Boss::AlienBoss::LIFE));
	shootClock = addComponent(new Game::Clock(*this));
}

void AlienBoss::_kill() {
	// TODO
}

void AlienBoss::update() {
	if (killable->isKilled()) return;
	
	if ((shotsFired > 0 && shootClock->getElapsedTime() > SHOOT_SHORT_INTERVAL)
			|| shootClock->getElapsedTime() > SHOOT_INTERVAL)
	{
		shootClock->restart();
		shotsFired = (shotsFired + 1) % N_SHOTS;
		_shoot();
	}
}

void AlienBoss::_shoot() {

}
