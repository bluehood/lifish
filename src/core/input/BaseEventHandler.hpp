#pragma once

#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "EventHandler.hpp"

namespace sf {
	class View;
class Window;
}

namespace lif {

class BaseEventHandler : public lif::EventHandler {
	sf::View _keepRatio(const sf::Event::SizeEvent& size, const sf::Vector2u& designedsize);

public:
	bool handleEvent(sf::Window& window, sf::Event event) override;
};

}
