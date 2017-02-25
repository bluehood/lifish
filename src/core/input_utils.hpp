#pragma once

#include <string>
#include <SFML/Window.hpp>

namespace lif {

namespace kb {
	/** Converts a key like Num0/Numpad0 to the respective number */
	short keyToNumber(sf::Keyboard::Key key);

	/** @return The display name of the given key */
	std::string keyToString(sf::Keyboard::Key key);
}

namespace joystick {
	enum class ButtonType {
		START,
		SELECT
	};
	short getButton(ButtonType type, unsigned joystickId);
}

}
