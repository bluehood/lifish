#include <SFML/Window/Joystick.hpp>
#include <string>
#include <sstream>
#include <array>

#include "game.hpp"
#include "controls.hpp"
#include "collision_layers.hpp"
#include "core.hpp"

bool lif::init() {
	if (!lif::initCore()) return false;
	for (unsigned i = 0; i < lif::MAX_PLAYERS; ++i) {
		if (sf::Joystick::isConnected(i))
			lif::controls::useJoystick[i] = i;
		else
			lif::controls::useJoystick[i] = -1;
	}

	// Setup collision layers
	lif::c_layers::init();

	return true;
}

std::string lif::gameInfo() {
	std::stringstream ss;
	ss << "lifish v." VERSION " rev." COMMIT;
#ifdef RELEASE
	ss << " RELEASE";
#endif
#ifndef ARCH
	ss << " (unknown arch)";
#else
	ss << " (" ARCH " bit)";
#endif
	ss << " by Giacomo Parolini\r\n";
#ifdef HAVE_NFD
	ss << "    | NFD support: yes\r\n";
#elif !defined(SFML_SYSTEM_WINDOWS)
	ss << "    | NFD support: no\r\n";
#endif
#ifdef MULTITHREADED
	ss << "    | Multithreaded: yes\r\n";
#else
	ss << "    | Multithreaded: no\r\n";
#endif
	return ss.str();
}
