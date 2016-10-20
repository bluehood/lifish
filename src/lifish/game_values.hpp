#pragma once

#include <random>
#include <SFML/System/Time.hpp>

/** This file basically contains numeric values for the gameplay-related (balanceable) constants */
namespace Game {

namespace Conf {

	const sf::Time EXTRA_GAME_DURATION = sf::seconds(30);

	namespace Player {
		constexpr unsigned INITIAL_LIVES = 4;
		constexpr unsigned MAX_LIFE = 16;
		constexpr unsigned DEFAULT_MAX_BOMBS = 5;
		constexpr unsigned MAX_MAX_BOMBS = 8;
		constexpr unsigned INITIAL_CONTINUES = 3;
		constexpr float DEFAULT_SPEED = 150.f;
		/** The number of letters needed to get an extra life */
		constexpr unsigned N_EXTRA_LETTERS = 5;
		const sf::Time DEATH_TIME = sf::seconds(5);
		const sf::Time DEATH_STOP_ANIM_TIME = sf::seconds(2);
		const sf::Time HURT_ANIM_DURATION = sf::milliseconds(120);
		const sf::Time RESURRECT_SHIELD_TIME = sf::seconds(5);
		const sf::Time DAMAGE_SHIELD_TIME = sf::seconds(1);
	}
	
	namespace Enemy {
		const sf::Time DEATH_TIME = sf::seconds(2);
	}

	namespace Wall {
		namespace Breakable {
			constexpr unsigned VALUE = 10;
		}
	}

	namespace Bomb {
		const sf::Time DEFAULT_FUSE = sf::seconds(1);
		const sf::Time QUICK_FUSE = DEFAULT_FUSE / 2.f;
		constexpr unsigned DEFAULT_RADIUS = 2;
		constexpr unsigned MAX_RADIUS = 4;
	}

	namespace Teleport {
		const sf::Time COOLDOWN_TIME = sf::seconds(1);
	}

	namespace ZIndex {
		// Default layer is 0
		constexpr int EXPLOSIONS     = 1;
		constexpr int TRANSP_WALLS   = 2;
		constexpr int WALLS          = 4;   
		constexpr int PLAYERS        = 5;
		constexpr int ENEMIES        = 6;
		constexpr int BULLETS        = 7;
		constexpr int BOSSES         = 7;
		constexpr int POINTS         = 42;
		constexpr int DROPPING_TEXTS = 42;
	}

	namespace Bullet {
		constexpr float BASE_SPEED = 200.;
	}

	namespace Bonus {
		constexpr unsigned VALUE = 100; // FIXME
		constexpr unsigned N_BONUS_TYPES = 9;
		constexpr unsigned N_PERMANENT_BONUS_TYPES = 5;
		const sf::Time EXPIRE_TIME = sf::seconds(10);
		const sf::Time SHIELD_DURATION = sf::seconds(20);
		const sf::Time SPEEDY_DURATION = sf::seconds(20);

		extern std::discrete_distribution<unsigned> distribution;
	}

	namespace Boss {
		namespace AlienBoss {
			constexpr unsigned VALUE = 5000;
			constexpr unsigned LIFE = 20;
			const sf::Time SHOOT_INTERVAL = sf::seconds(2);
			const sf::Time SHOOT_SHORT_INTERVAL = sf::milliseconds(400);
			constexpr unsigned N_SHOTS = 3;
			constexpr float SIGHT_RADIUS = 9;
		}
		namespace BigAlienBoss {
			constexpr unsigned VALUE = 50000;
			constexpr unsigned LIFE = 100;
		}
	}
}

}
