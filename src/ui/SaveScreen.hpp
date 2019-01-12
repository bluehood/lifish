#pragma once

#include <stddef.h>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <string>

#include "Screen.hpp"

namespace sf {

class RectangleShape;
class Event;
class RenderWindow;
class Window;

}

namespace lif {

class ShadedText;

namespace ui {

class SaveScreen : public lif::ui::Screen {
public:
	static constexpr int MAX_SAVE_FILE_NAME_LEN = 10;

private:
	std::array<char, MAX_SAVE_FILE_NAME_LEN> buffer;
	size_t bufIdx = 0;

	sf::RectangleShape *cursor = nullptr;
	sf::Clock cursorClock;
	bool cursorVisible = true;
	sf::Vector2f charBounds;

	lif::ShadedText *bufferText = nullptr;
	lif::ShadedText *errText = nullptr;

	void _updateCursorPosition();

public:
	static constexpr const char *SCREEN_NAME = "save";

	explicit SaveScreen(const sf::RenderWindow& window, const sf::Vector2u& size);

	std::string getPrompt() const;

	bool handleEvent(sf::Window&, sf::Event) override;
	void update() override;
	void onLoad() override;
};

}

}
