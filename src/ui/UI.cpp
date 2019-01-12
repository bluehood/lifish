#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <algorithm>
#include <functional>

#include "input_utils.hpp"
#include "UI.hpp"
#include "contexts.hpp"
#include "Screen.hpp"
#include "BaseEventHandler.hpp"
#include "screen_callbacks.hpp"
#include "SaveScreen.hpp"
#include "WithOrigin.hpp"
#include "core.hpp"

namespace sf {
class RenderWindow;
class Window;
}  // namespace sf

using lif::ui::UI;

UI::UI() : lif::WindowContext() {
	_addHandler<lif::BaseEventHandler>();
}

void UI::load(const sf::RenderWindow& window, std::initializer_list<std::string> scrNames) {
	for (const auto& name : scrNames) {
		if (screens.find(name) != screens.end()) {
			std::cerr << "[ WARNING ] Screen " << name << " already loaded: skipping." << std::endl;
			continue;
		}
		auto screen = new lif::ui::Screen(name, window, size);
		if (curScreen == nullptr) {
			curScreen = screen;
			curScreen->setOrigin(origin);
		}
		screens[screen->getName()] = std::unique_ptr<lif::ui::Screen>(screen);
	}
}

bool UI::handleEvent(sf::Window& window, sf::Event event) {
	if (curScreen == nullptr)
		return false;

	if (curScreen->pollTriggeredAction()) {
		_processAction(curScreen->getTriggeredAction());
		return true;
	}

	if (curScreen->handleEvent(window, event))
		return true;

	// In case the current screen is expecting an event, tell it.
	switch (event.type) {
	case sf::Event::MouseButtonReleased:
		fireClick();
		return true;
	case sf::Event::JoystickButtonPressed:
		{
			const auto btn = event.joystickButton;
			if (lif::joystick::isButton(lif::joystick::ButtonType::START, btn.joystickId, btn.button)) {
				if (getCurrent() == "pause")
					newContext = lif::CTX_GAME;
			} else if (curScreen->isUsingJoystick()) {
				if (lif::joystick::isButton(
						lif::joystick::ButtonType::BTN_DOWN, btn.joystickId, btn.button))
				{
					fireClick();
				} else if (lif::joystick::isButton(
						lif::joystick::ButtonType::BTN_RIGHT, btn.joystickId, btn.button))
				{
					setCurrentToParent();
				}
			}
			return true;
		}
	case sf::Event::KeyPressed:
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			setCurrentToParent();
			return true;
		case sf::Keyboard::P:
			active = !active;
			return true;
		default:
			break;
		}
	default:
		break;
	}
	return false;
}

void UI::setOrigin(const sf::Vector2f& pos) {
	lif::WithOrigin::setOrigin(pos);
}

void UI::update() {
	quitGame = false;
	loadGame = false;
	saveGame = false;
	if (curScreen != nullptr)
		curScreen->update();
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (curScreen != nullptr)
		target.draw(*curScreen, states);
}

void UI::fireClick() {
	if (curScreen == nullptr) return;
	auto cbname = curScreen->getSelected();

	lif::ui::Action action;

	// Check if screen has an internal registered callback for that element; if not, call an external one.
	if (curScreen->hasCallback(cbname)) {
		action = curScreen->fireCallback(cbname);
	} else {
		auto it = lif::ui::screenCallbacks.find(cbname);
		if (it == lif::ui::screenCallbacks.end()) return;
		action = it->second();
	}

	_processAction(action);
}

void UI::_processAction(lif::ui::Action action) {
	switch (action) {
	case Action::EXIT:
		// terminate the game
		lif::exitCode = 0;
		lif::terminated = true;
		break;
	case Action::SAVE_GAME:
		saveGame = true;
		saveName = static_cast<lif::ui::SaveScreen*>(curScreen)->getPrompt();
		setCurrentToParent();
		break;
	case Action::LOAD_GAME:
		loadGame = true;
		newContext = lif::CTX_INTERLEVEL;
		break;
	case Action::QUIT_TO_HOME:
		quitGame = true;
		setCurrent("home");
		break;
	case Action::SWITCH_SCREEN:
		setCurrent(lif::ui::screenCallbackArg);
		break;
	case Action::SWITCH_SCREEN_OVERRIDE_PARENT:
		setCurrent(lif::ui::screenCallbackArg, true);
		break;
	case Action::SWITCH_TO_PARENT:
		setCurrentToParent();
		break;
	case Action::START_GAME:
		newContext = lif::CTX_INTERLEVEL;
		break;
	case Action::DEACTIVATE_UI:
		newContext = lif::CTX_GAME;
		break;
	default:
		break;
	}
}

void UI::setCurrent(const std::string& name, bool overrideParent) {
	if (name.length() == 0) return;

	const auto oldScreen = curScreen->getName();
	const auto it = screens.find(name);
	if (it == screens.end())
		throw std::invalid_argument("Screen " + name + " was not loaded!");
	curScreen = it->second.get();
	curScreen->setOrigin(origin);
	if (overrideParent)
		curScreen->setParent(oldScreen);
	curScreen->onLoad();
}

void UI::setCurrentToParent() {
	setCurrent(curScreen->getParent());
}

std::string UI::getCurrent() const {
	if (curScreen == nullptr) return "";
	return curScreen->getName();
}
