#pragma once

#include <array>
#include <SFML/System.hpp>
#include "JoystickListener.hpp"
#include "EventHandler.hpp"

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

	const lif::joystick::JoystickListener& getListener(unsigned n) const { return listeners[n]; }
	bool isAnyEvtMoved() const;	

	void update();

	bool handleEvent(sf::Window& window, sf::Event evt) override;
};

}

}
