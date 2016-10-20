#include "AxisBullet.hpp"
#include "Attack.hpp"
#include "AxisMoving.hpp"
#include "Animated.hpp"
#include "Drawable.hpp"
#include "Collider.hpp"
#include "Killable.hpp"
#include "game.hpp"

using Game::AxisBullet;
using Game::TILE_SIZE;

AxisBullet::AxisBullet(const sf::Vector2f& pos, const Game::Entity *const source, 
		const Game::Direction dir, const Game::Attack& attack)
	: Game::Bullet(pos, source, attack)
{
	// Bullets have a variable number of frames, up to 13:
	// motion frames: 1 ~ 8 (max 8 / directional per direction)
	// destroy frames: 0 ~ 5
	// TODO: refactor?
	AxisBulletPresets::setup(*this, attack.id);

	unsigned d = 0;
	switch (dir) {
	case Game::Direction::DOWN: 
		d = 0;
		position.x += (TILE_SIZE - size) / 2;
		position.y += TILE_SIZE;
		break;
	case Game::Direction::UP:    
		d = directionality == 4 ? 1 : 0;
		position.x += (TILE_SIZE - size) / 2;
		position.y -= size;
		break;
	case Game::Direction::RIGHT:
		d = directionality == 4 ? 2 : directionality == 2 ? 1 : 0; 
		position.y += (TILE_SIZE - size) / 2;
		position.x += TILE_SIZE;
		break;
	case Game::Direction::LEFT: 
		d = directionality == 4 ? 3 : directionality == 2 ? 1 : 0;
		position.y += (TILE_SIZE - size) / 2;
		position.x -= size;
		break;
	default: 
		break;
	}

	collider = addComponent(new Game::Collider(*this, [this] (Game::Collider&) {
		// on collision
		auto klb = get<Game::Killable>();
		if (!klb->isKilled()) {
			klb->kill();
		}
	}, Game::Layers::ENEMY_BULLETS, sf::Vector2i(size, size)));
	auto moving = addComponent(new Game::AxisMoving(*this, BASE_SPEED * attack.speed, dir));
	moving->setEnsureAlignEnabled(false);
	moving->setAutoRealignEnabled(false);
	auto animated = addComponent(new Game::Animated(*this, Game::getAsset("test", "bullets.png")));
	addComponent(new Game::Drawable(*this, *animated));

	auto& a_move = animated->addAnimation("move");
	auto& a_destroy = animated->addAnimation("destroy");

	// Since the bullet cannot change direction, we only need to setup these 2 animations.
	// The motion animation has up to 8 / directionality frames (but can also have less),
	// while the destroy animation can have from 0 to 5 frames.
	// Format for the spritesheet is:
	// 	- if directionality == 1, [motion frames] [destroy frames]
	//	- if == 2, [up/down frames] [left/right frames] [destroy frames]
	//	- if == 4, [down] [up] [right] [left] [destroy]
	for (unsigned i = 0; i < nMotionFrames && i < 8 / directionality; ++i)
		a_move.addFrame(sf::IntRect(
				(nMotionFrames * d + i) * TILE_SIZE, 
				(attack.id - 1) * TILE_SIZE, 
				TILE_SIZE, 
				TILE_SIZE));

	// destroy animations are non-directional
	for (unsigned i = 0; i < nDestroyFrames && i < 5; ++i)
		a_destroy.addFrame(sf::IntRect(
				(nMotionFrames * directionality + i) * TILE_SIZE, 
				(attack.id-1) * TILE_SIZE, 
				TILE_SIZE, 
				TILE_SIZE));

	auto& animatedSprite = animated->getSprite();
	animatedSprite.setAnimation(a_move);
	animatedSprite.setLooped(true);
	animatedSprite.setFrameTime(sf::seconds(0.10));
	animatedSprite.play();
}

/// AxisBulletPresets
void Game::AxisBulletPresets::setup(Game::AxisBullet& b, unsigned id) {
	switch (id) {
	case 1:
		// shot
		b.directionality = 1;
		b.size = 7;
		break;
	case 2:
		// fireball
		b.directionality = 1;
		b.size = 13;
		break;
	case 3:
		// MG shot
		b.directionality = 4;
		b.nMotionFrames = 1;
		b.nDestroyFrames = 5;
		b.size = 10;
		break;
	case 4:
		// lightbolt
		b.directionality = 1;
		b.size = 13;
		break;
	case 5:
		// flame
		b.directionality = 2;
		b.nMotionFrames = 5;
		b.nDestroyFrames = 0;
		b.size = TILE_SIZE;
		break;
	case 6:
		// plasma
		b.directionality = 1;
		b.size = 12;
		break;
	case 7:
		// magma
		b.directionality = 4;
		b.size = 26;
		break;
	default:
		break;
	}
}
