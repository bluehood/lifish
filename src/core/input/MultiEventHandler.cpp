#include <SFML/Window/Window.hpp>

#include "MultiEventHandler.hpp"

using lif::MultiEventHandler;

void MultiEventHandler::handleEvents(sf::Window& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (handleEvent(window, event))
			return;

		for (auto& handler : handlers)
			if (handler->handleEvent(window, event))
				return;
	}
}
