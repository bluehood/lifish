#pragma once

#include <SFML/System.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Joystick.hpp>
#include <array>

#include "JoystickListener.hpp"
#include "EventHandler.hpp"

namespace sf {
class Event;
class Window;
}  // namespace sf

namespace lif {

namespace joystick {

class JoystickManager final : public lif::EventHandler, private sf::NonCopyable {
	std::array<lif::joystick::JoystickListener, sf::Joystick::Count> listeners;

	JoystickManager();
public:
	static JoystickManager& getInstance() {
		static JoystickManager instance;
		return instance;
	}

	lif::joystick::JoystickListener& getListener(unsigned n) { return listeners[n]; }
	/** @return -1 if no evtMoved, else the index of the first listener that has an evtMoved */
	int isAnyEvtMoved() const;

	void update();

	bool handleEvent(sf::Window& window, sf::Event evt) override;
};

}

}
