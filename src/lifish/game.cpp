#include "game.hpp"
#include "LoopingMusic.hpp"
#include "controls.hpp"
#include "collision_layers.hpp"
#include <cstring>
#include <iostream>
#include <array>
#include "game_values.hpp"

std::array<unsigned, Game::MAX_PLAYERS> Game::score;
std::array<unsigned, Game::MAX_PLAYERS> Game::playerContinues;

bool Game::init() {
	if (!Game::initCore()) return false;
	Game::playerContinues.fill(Game::Conf::Player::INITIAL_CONTINUES);
	Game::Controls::useJoystick.fill(-1);

	// Setup collision layers
	Game::Layers::init();

	return true;
}
