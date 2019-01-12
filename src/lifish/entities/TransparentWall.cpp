#include <memory>
#include <unordered_map>

#include "TransparentWall.hpp"
#include "Collider.hpp"
#include "ZIndexed.hpp"
#include "Sounded.hpp"
#include "conf/zindex.hpp"
#include "Entity.inl"
#include "collision_layers.hpp"
#include "core.hpp"

using lif::TransparentWall;

TransparentWall::TransparentWall(const sf::Vector2f& pos)
	: BreakableWall(pos, lif::getAsset("graphics", "transparent_wall.png"), 1, SCORE)
{
	get<lif::Collider>()->setLayer(lif::c_layers::TRANSP_WALLS);
	get<lif::ZIndexed>()->setZIndex(lif::conf::zindex::TRANSP_WALLS);
	get<lif::Sounded>()->setSoundFile("death", lif::getAsset("sounds", "transp_wall_break.ogg"));
}
