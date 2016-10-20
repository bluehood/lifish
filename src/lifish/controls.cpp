#include "controls.hpp"

std::array<sf::Keyboard::Key, Game::Controls::CONTROLS_NUM> Game::Controls::players[] =
{
	{{
		/* Control::UP,   */ sf::Keyboard::Key::Up,
		/* Control::DOWN, */ sf::Keyboard::Key::Down,
		/* Control::LEFT, */ sf::Keyboard::Key::Left,
		/* Control::RIGHT,*/ sf::Keyboard::Key::Right,
		/* Control::BOMB, */ sf::Keyboard::Key::RControl
	}},
	{{
		/* Control::UP,    */ sf::Keyboard::Key::W,
		/* Control::DOWN,  */ sf::Keyboard::Key::S,
		/* Control::LEFT,  */ sf::Keyboard::Key::A,
		/* Control::RIGHT, */ sf::Keyboard::Key::D,
		/* Control::BOMB,  */ sf::Keyboard::Key::LControl
	}}
};

std::array<unsigned, Game::MAX_PLAYERS> Game::Controls::joystickBombKey = {{ 0, 0 }};
std::array<int, Game::MAX_PLAYERS> Game::Controls::useJoystick;
