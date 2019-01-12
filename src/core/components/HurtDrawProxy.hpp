#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>

#include "Component.hpp"
#include "Entity.hpp"

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

class Animated;

/**
 * This class can be used as a lif::Drawable to provide the "hurt effect"
 * that changes the owner's sprite color to red for a moment after being hurt
 * (used by bosses etc).
 */
class HurtDrawProxy : public lif::Component, public sf::Drawable {
	lif::Animated *animated = nullptr;
	sf::Time hurtT = sf::Time::Zero;

public:
	/** `owner` must have an Animated component */
	explicit HurtDrawProxy(lif::Entity& owner, lif::Animated *animated = nullptr);

	/** Signal that the owner was hurt, resets hurtClock */
	void hurt();
	bool isHurt() const;

	lif::Entity* init() override;
	void update() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}
