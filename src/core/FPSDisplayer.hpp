#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

#include "ShadedText.hpp"

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

class FPSDisplayer : public sf::Drawable {

	lif::ShadedText fpsText;
	sf::Clock fpsClock, fpsUpdateClock;
	double curTime = 0;
	int nUpdates = 0;

public:
	explicit FPSDisplayer(const sf::Vector2f& pos, const std::string& fontname);

	void update();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

}
