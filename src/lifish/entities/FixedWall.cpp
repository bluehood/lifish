#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "FixedWall.hpp"
#include "Sprite.hpp"
#include "Drawable.hpp"
#include "Collider.hpp"
#include "Fixed.hpp"
#include "ZIndexed.hpp"
#include "conf/zindex.hpp"
#include "Entity.inl"
#include "collision_layers.hpp"
#include "core.hpp"

using lif::FixedWall;
using lif::TILE_SIZE;

FixedWall::FixedWall(const sf::Vector2f& pos, const unsigned short id)
	: lif::Entity(pos)
{
	addComponent<lif::Fixed>(*this);
	addComponent<lif::ZIndexed>(*this, lif::conf::zindex::WALLS);
	addComponent<lif::Drawable>(*this,
			*addComponent<lif::Sprite>(*this, lif::getAsset("graphics", "fixed.png"),
				sf::IntRect((id-1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE)));
	addComponent<lif::Collider>(*this, lif::c_layers::UNBREAKABLES);
}
