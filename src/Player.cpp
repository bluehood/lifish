#include "Player.hpp"
#include "utils.hpp"
#include <sstream>

using Game::Player;

Player::Player(sf::Vector2f pos, const unsigned short id) :
	Game::LifedMovingEntity(
		pos,
		Game::getAsset("graphics", std::string("player") + Game::to_string(id) + std::string(".png")), 
		MAX_LIFE,
		{
			Game::getAsset("sounds", std::string("P") + Game::to_string(id) + std::string("Death.wav")),
			Game::getAsset("sounds", std::string("P") + Game::to_string(id) + std::string("Ouch.wav")),
			Game::getAsset("sounds", std::string("P") + Game::to_string(id) + std::string("Complete.wav")),
		})
{
	speed = 150.f;
	transparentTo.players = false;
	remainingLives = INITIAL_LIVES;
	extra.fill(false);

	for (unsigned short i = 0; i < MAX_N_ANIMATIONS; ++i)
		animations[i].setSpriteSheet(*texture);

	for (unsigned short i = 0; i < WALK_N_FRAMES; ++i) {
		animations[ANIM_DOWN].addFrame(sf::IntRect(i * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
		animations[ANIM_UP].addFrame(sf::IntRect(i * TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
		animations[ANIM_RIGHT].addFrame(sf::IntRect(i * TILE_SIZE, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		animations[ANIM_LEFT].addFrame(sf::IntRect(i * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		if (i > DEATH_N_FRAMES + 1) continue;
		unsigned short k = ANIM_DEATH;
		if (i == DEATH_N_FRAMES) k = ANIM_WIN;
		else if (i == DEATH_N_FRAMES + 1) k = ANIM_HURT;
		animations[k].addFrame(sf::IntRect(i * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}

	animatedSprite.setAnimation(animations[ANIM_DOWN]);
	animatedSprite.setLooped(true);
	animatedSprite.setFrameTime(sf::seconds(0.06));
	animatedSprite.pause();
}

void Player::move() {
	move(direction);
}

void Player::move(const Game::Direction dir) {
	MovingEntity::move(dir);
	if (colliding)
		realign();
}

bool Player::_isTransparentTo(const Entity *const e) const {
	return e->transparentTo.players;
}

void Player::resurrect() {
	dead = false;
	animatedSprite.setAnimation(animations[ANIM_DOWN]);
	animatedSprite.pause();
	life = MAX_LIFE;
	extra.fill(false);
	frameClock.restart();
	realign();
}

bool Player::playDeathAnimation() {
	float time = deathClock.getElapsedTime().asMilliseconds();
	if (time > 3000) {
		animatedSprite.setFrame(1);
		animatedSprite.pause();
	} else {
		animatedSprite.update(frameClock.restart());
	}
	return time < DEATH_TIME;
}

void Player::kill() {
	MovingEntity::kill();
	life = 0;
	powers.bombRadius = Game::Bomb::DEFAULT_RADIUS;
	powers.bombFuseTime = Game::Bomb::DEFAULT_FUSE;
	powers.maxBombs = DEFAULT_MAX_BOMBS;
	speedyTime = 0;
	shieldTime = 0;
}

void Player::stop() {
	MovingEntity::stop();
	if (winning) {
		animatedSprite.setAnimation(animations[ANIM_WIN]);
		animatedSprite.update(frameClock.restart());
	}
}
