#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <random>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "BossExplosion.hpp"
#include "Animated.hpp"
#include "ZIndexed.hpp"
#include "Sounded.hpp"
#include "conf/zindex.hpp"
#include "AnimatedSprite.hpp"
#include "Entity.inl"
#include "core.hpp"
#include "sid.hpp"

using lif::BossExplosion;
using lif::TILE_SIZE;

BossExplosion::BossExplosion(const sf::Vector2f& pos)
	: lif::OneShotFX(pos, "blink.png", {
		sf::IntRect(0 * 2 * TILE_SIZE, 0, 2 * TILE_SIZE, 2 * TILE_SIZE),
		sf::IntRect(1 * 2 * TILE_SIZE, 0, 2 * TILE_SIZE, 2 * TILE_SIZE),
		sf::IntRect(2 * 2 * TILE_SIZE, 0, 2 * TILE_SIZE, 2 * TILE_SIZE),
		sf::IntRect(3 * 2 * TILE_SIZE, 0, 2 * TILE_SIZE, 2 * TILE_SIZE),
		sf::IntRect(4 * 2 * TILE_SIZE, 0, 2 * TILE_SIZE, 2 * TILE_SIZE),
	})
{
	addComponent<lif::Sounded>(*this,
		lif::sid("explode"), lif::getAsset("test", "bossbullet_hit.ogg")
	);
	get<lif::ZIndexed>()->setZIndex(lif::conf::zindex::BOSS_EXPLOSIONS);
	std::uniform_int_distribution<> dist(100, 232);
	animated->getSprite().setColor(sf::Color(232, dist(lif::rng), 0));
}
