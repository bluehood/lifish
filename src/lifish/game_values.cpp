#include "game_values.hpp"

/** Relative bonus probabilities */
std::discrete_distribution<unsigned> Game::Conf::Bonus::distribution {
	5,   // max bombs
	4,   // quick fuse
	5,   // max range
	5,   // shield
	5,   // speedy
	1,   // zapper
	1,   // sudden death
	5,   // health small
	4,   // health full
	490  // no bonus
};
