#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

class LevelManager;

class LevelRenderer final : public sf::Drawable {
	lif::LevelManager& owner;

public:
	explicit LevelRenderer(lif::LevelManager& owner);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}
