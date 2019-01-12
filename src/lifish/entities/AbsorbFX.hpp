#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <memory>

#include "Entity.hpp"

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

class Sprite;

class AbsorbFX : public lif::Entity, public sf::Drawable {

	std::weak_ptr<const lif::Entity> target;
	bool expired = false;

	std::array<lif::Sprite*, 3> sprites;

public:
	explicit AbsorbFX(const sf::Vector2f& pos, std::weak_ptr<const lif::Entity> target);

	void update() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}
