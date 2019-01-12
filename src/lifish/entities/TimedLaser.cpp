#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector2.inl>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include "TimedLaser.hpp"
#include "Animated.hpp"
#include "Collider.hpp"
#include "Drawable.hpp"
#include "Lifed.hpp"
#include "Temporary.hpp"
#include "Time.hpp"
#include "ZIndexed.hpp"
#include "conf/zindex.hpp"
#include "game.hpp"
#include "AnimatedSprite.hpp"
#include "Entity.inl"
#include "core.hpp"

using lif::TimedLaser;
using lif::TILE_SIZE;

TimedLaser::TimedLaser(unsigned short rowCol,
			TimedLaser::Orientation ori,
			const sf::Time& warnDuration,
			int _damage,
			std::initializer_list<lif::c_layers::Layer> _damagedLayers)
	: lif::Entity({0, 0})
	, rowCol(rowCol)
	, warnDuration(warnDuration)
	, damage(_damage)
	, damagedLayers(_damagedLayers)
{
	const bool isVert = ori == TimedLaser::Orientation::VERTICAL;

	animated = addComponent<lif::Animated>(*this, lif::getAsset("graphics", "timed_laser.png"));
	animated->addAnimation("warn_blink", {
		sf::IntRect(0 * TILE_SIZE, isVert * TILE_SIZE, TILE_SIZE, TILE_SIZE),
		sf::IntRect(1 * TILE_SIZE, isVert * TILE_SIZE, TILE_SIZE, TILE_SIZE)
	}, true);
	auto& sprite = animated->getSprite();
	sprite.setLooped(true);
	sprite.setFrameTime(sf::seconds(0.08));
	addComponent<lif::Drawable>(*this, *animated);
	addComponent<lif::ZIndexed>(*this, lif::conf::zindex::FLASHES);

	// Set the position
	sf::Vector2f bounds(TILE_SIZE, TILE_SIZE);
	if (isVert) {
		position = sf::Vector2f(TILE_SIZE * (rowCol + 1), TILE_SIZE);
		sprite.setScale(1, lif::GAME_HEIGHT);
		bounds.y *= lif::GAME_HEIGHT;
	} else {
		position = sf::Vector2f(TILE_SIZE, TILE_SIZE * (rowCol + 1));
		sprite.setScale(lif::GAME_WIDTH, 1);
		bounds.x *= lif::GAME_HEIGHT;
	}

	addComponent<lif::Collider>(*this, [this] (const lif::Collider& cld) {
		// on collision
		if (isWarning
			|| (damagedLayers.size() > 0
				&& std::find(damagedLayers.begin(), damagedLayers.end(), cld.getLayer())
					== damagedLayers.end())
			|| std::find(dmgEntities.begin(), dmgEntities.end(), &cld.getOwner()) != dmgEntities.end())
		{
			return;
		}
		auto& owner = cld.getOwnerRW();
		auto lifed = owner.get<lif::Lifed>();
		if (lifed == nullptr)
			return;
		lifed->decLife(damage);
		dmgEntities.emplace_back(&owner);
	}, lif::c_layers::DEFAULT, bounds);

	addComponent<lif::Temporary>(*this, [this] () {
		return animated->getSprite().getColor().a < 1;
	});
}

void TimedLaser::update() {
	lif::Entity::update();

	const auto delta = lif::time.getDelta();
	warnT += delta;
	if (isWarning && warnT > warnDuration) {
		isWarning = false;
		animated->getSprite().stop();
	}

	if (!isWarning) {
		auto& sprite = animated->getSprite();
		auto col = sprite.getColor();
		col.a -= delta.asSeconds();
		sprite.setColor(col);
	}
}
