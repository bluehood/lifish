#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>
#include <utility>

#include "ScreenStyle.hpp"
#include "Action.hpp"
#include "WindowContext.hpp"
#include "TransitionGraph.hpp"

namespace sf {
class Event;
class RenderTarget;
class RenderWindow;
class Window;
}  // namespace sf

namespace lif {

namespace ui {

class ScreenBuilder;
class Interactable;
struct ScreenStyle;

class Screen : public lif::WindowContext {
public:
	using Callback = std::function<lif::ui::Action()>;

private:
	friend class lif::ui::ScreenBuilder;

	sf::Vector2i latestMousePos;
	bool wasActionTriggered = false;
	lif::ui::Action actionTriggered = lif::ui::Action::DO_NOTHING;
	float itrScale = 0;
	sf::Clock itrScaleClock;
	bool itrScaleMid = false;

	void _saveMousePos(int x, int y);
	void _setItrScale(float x);
	void _updateItrScale();
	void _select(lif::ui::Interactable& elem);
	void _deselect();

protected:
	/** The window this screen is rendered in */
	const sf::RenderWindow& window;
	/** The expected size of the rendering target, used when positioning elements in this screen */
	sf::Vector2u size;
	/** The name of this screen */
	std::string name;
	/** The parent screen, if any */
	std::string parent;
	/** The background tile */
	sf::Sprite bgSprite;

	/** Whether this Screen was already built or not */
	bool built = false;
	/** If true, the elements' selection is being done via joystick, else via mouse */
	bool usingJoystick = false;

	/** The styles */
	std::unordered_map<std::string, lif::ui::ScreenStyle> styles;
	/** The interactable texts/images */
	std::unordered_map<std::string, std::unique_ptr<lif::ui::Interactable>> interactables;
	/** The non-interactable texts/images */
	std::vector<std::unique_ptr<sf::Drawable>> nonInteractables;

	/** The internal callbacks, if any. Internal callbacks have priority over external ones. */
	std::unordered_map<std::string, Callback> callbacks;

	/** The currently selected element, if any */
	std::pair<std::string, lif::ui::Interactable*> selected;

	/** The elements transition graph, used when navigating via joystick */
	lif::ui::TransitionGraph transitions;


	Screen(const sf::RenderWindow& window, const sf::Vector2u& size);
	void _loadBGSprite(const std::string& bgSpritePath);
	void _updateSelectedMouse();
	void _updateSelectedJoystick();
	/** Manually triggers an Action, as if an interactable was clicked. Useful for
	 *  raising actions upon keyboard shortcuts and similar.
	 */
	void _triggerAction(lif::ui::Action action);

public:
	explicit Screen(const std::string& layoutFileName, const sf::RenderWindow& window, const sf::Vector2u& size);

	bool wasBuilt() const { return built; }
	void setParent(const std::string& name) { parent = name; }
	const std::string& getParent() const { return parent; }
	const std::string& getName() const { return name; }

	/** @return The name of the selected element, if any (else "") */
	std::string getSelected() const;

	bool isUsingJoystick() const { return usingJoystick; }

	bool hasCallback(const std::string& name) const;
	lif::ui::Action fireCallback(const std::string& name);

	/** @return Whether we have triggered an action or not. Upon this query, the action trigger resets. */
	bool pollTriggeredAction() { bool t = wasActionTriggered; return wasActionTriggered = false, t; }
	lif::ui::Action getTriggeredAction() const { return actionTriggered; }

	/** This may be used by child classes to do specific logic;
	 *  @return true if signal was caught and should be ignored by UI's event loop.
	 */
	virtual bool handleEvent(sf::Window&, sf::Event) override;
	virtual void update() override;
	/** Called whenever the screen is displayed */
	virtual void onLoad();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setOrigin(const sf::Vector2f& pos) override;
};

}

}
