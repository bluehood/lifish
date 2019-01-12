#pragma once

#include <SFML/System/Vector2.hpp>

#include "Screen.hpp"
#include "SaveDataBrowser.hpp"

namespace sf {
class RenderWindow;
}  // namespace sf

namespace lif {

namespace ui {

class LoadScreen : public lif::ui::Screen, public lif::SaveDataBrowser {
public:
	static constexpr const char *SCREEN_NAME = "load";

	explicit LoadScreen(const sf::RenderWindow& window, const sf::Vector2u& size);

	/** Reads the save directory and presents all the save files found in the screen */
	void onLoad() override;
};

}

}
