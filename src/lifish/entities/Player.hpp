#pragma once

#include <array>
#include "Entity.hpp"
#include "game.hpp"
#include "game_values.hpp"

namespace Game {

class Bonusable;
class Killable;
class Clock;
class MovingAnimator;
class Clock;
class AxisMoving;
class Collider;
class Animated;
class RegularEntityDeath;

/** This structs contains all the data which is persistent through different levels. */
struct PlayerInfo {
	unsigned id;
	struct {
		unsigned bombRadius = Game::Conf::Bomb::DEFAULT_RADIUS;
		unsigned maxBombs   = Game::Conf::Player::DEFAULT_MAX_BOMBS;
		sf::Time bombFuseTime     = Game::Conf::Bomb::DEFAULT_FUSE;
	} powers;

	int remainingLives = Game::Conf::Player::INITIAL_LIVES - 1;

	/** The EXTRA letters of this player */
	std::array<bool, Game::Conf::Player::N_EXTRA_LETTERS> extra;

	PlayerInfo(unsigned id) : id(id) {
		extra.fill(false);
	}

	void reset(bool resetExtra) {
		if (resetExtra)
			extra.fill(false);
		powers.bombRadius = Game::Conf::Bomb::DEFAULT_RADIUS;
		powers.maxBombs = Game::Conf::Player::DEFAULT_MAX_BOMBS;
		powers.bombFuseTime = Game::Conf::Bomb::DEFAULT_FUSE;
	}
};

class Player;

class PlayerDrawProxy : public sf::Drawable {
	const Game::Player& player;
public:
	explicit PlayerDrawProxy(const Game::Player& player);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

/**
 * The player entity
 */
class Player : public Game::Entity {

	friend class Game::PlayerDrawProxy;

	constexpr static unsigned WALK_N_FRAMES = 8;
	constexpr static unsigned DEATH_N_FRAMES = 3;
	constexpr static unsigned IDLE_N_FRAMES = 20;
	const static sf::Time DEATH_TIME;

	/** While true, the idle pose becomes ANIM_WIN */
	bool winning = false;

	Game::AxisMoving *moving = nullptr;
	Game::Animated *animated = nullptr;
	Game::Bonusable *bonusable = nullptr;
	Game::MovingAnimator *movingAnimator = nullptr;
	Game::Killable *killable = nullptr;
	Game::RegularEntityDeath *death = nullptr;
	Game::Clock *hurtClock = nullptr;

	Game::PlayerInfo info;
	PlayerDrawProxy drawProxy;

	void _init();
	void _kill();
	void _hurt();
	void _checkCollision(Game::Collider& cld);

public:
	/** Creates a player with the default state and id `id` */
	explicit Player(const sf::Vector2f& pos, const unsigned id);
	/** Creates a player whose state is described by `info` */
	explicit Player(const sf::Vector2f& pos, const Game::PlayerInfo& info);

	void resurrect();

	void setWinning(bool b) { winning = b; }

	const Game::PlayerInfo& getInfo() const { return info; }
	void setBombRadius(unsigned r) { info.powers.bombRadius = r; }
	void setMaxBombs(unsigned m) { info.powers.maxBombs = m; }
	void setBombFuseTime(sf::Time t) { info.powers.bombFuseTime = t; }
	void setExtra(unsigned n, bool e) { info.extra[n] = e; }
	void setRemainingLives(int l) { info.remainingLives = l; }

	void update() override;
};

}
