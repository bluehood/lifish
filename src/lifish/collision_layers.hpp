#pragma once

#include "utils.hpp"

namespace Game {

namespace Layers {

enum Layer : unsigned {
	DEFAULT,
	PLAYERS,
	ENEMIES,
	BOSSES,
	ENEMY_BULLETS,
	BOSS_BULLETS,
	WALLS,
	TRANSP_WALLS,
	TELEPORTS,
	EXPLOSIONS,
	BOMBS,
	GRABBABLE,
	N_LAYERS
};

extern Matrix<bool, Layers::N_LAYERS, Layers::N_LAYERS> collide, // whether levels "see" each other
                                                        solid;   // whether levels are solid for each other

/** To be called by Game::init() */
void init();

}

}
