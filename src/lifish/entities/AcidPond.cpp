#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "AcidPond.hpp"
#include "Sprite.hpp"
#include "Drawable.hpp"
#include "Entity.inl"
#include "collision_layers.hpp"
#include "core.hpp"

using lif::AcidPond;

AcidPond::AcidPond(const sf::Vector2f& pos, const sf::Vector2f& size)
	: lif::Pond(pos, size, 1, { lif::c_layers::PLAYERS })
{
	auto sprite = addComponent<lif::Sprite>(*this,
				lif::getAsset("graphics", "acid_pond.png"),
				sf::IntRect(0, 0, size.x, size.y));
	sprite->getTexture()->setRepeated(true);
	addComponent<lif::Drawable>(*this, *sprite);
}
