#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Rect.inl>
#include <SFML/System/Vector2.inl>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Teleport.hpp"
#include "Animated.hpp"
#include "AxisMoving.hpp"
#include "Collider.hpp"
#include "Drawable.hpp"
#include "Fixed.hpp"
#include "GameCache.hpp"
#include "OneShotFX.hpp"
#include "Sounded.hpp"
#include "Spawning.hpp"
#include "Time.hpp"
#include "conf/teleport.hpp"
#include "AnimatedSprite.hpp"
#include "Animation.hpp"
#include "Entity.inl"
#include "collision_layers.hpp"
#include "core.hpp"
#include "sid.hpp"
#include "utils.hpp"

using lif::Teleport;
using lif::TILE_SIZE;

Teleport::Teleport(const sf::Vector2f& pos)
	: lif::Entity(pos)
{
	addComponent<lif::Fixed>(*this);
	animated = addComponent<lif::Animated>(*this, lif::getAsset("graphics", "teleport.png"));
	addComponent<lif::Drawable>(*this, *animated);
	collider = addComponent<lif::Collider>(*this, [this] (lif::Collider& c) {
		_warp(c);
	}, lif::c_layers::TELEPORTS);
	addComponent<lif::Spawning>(*this, [this] (const lif::Spawning&) {
		return mustSpawnFlash;
	}, [this] () {
		mustSpawnFlash = false;
		return new lif::OneShotFX(position, "flash.png", {
			sf::IntRect(0, 0, lif::TILE_SIZE, lif::TILE_SIZE),
			sf::IntRect(lif::TILE_SIZE, 0, lif::TILE_SIZE, lif::TILE_SIZE),
			sf::IntRect(2 * lif::TILE_SIZE, 0, lif::TILE_SIZE, lif::TILE_SIZE),
			sf::IntRect(3 * lif::TILE_SIZE, 0, lif::TILE_SIZE, lif::TILE_SIZE),
			sf::IntRect(2 * lif::TILE_SIZE, 0, lif::TILE_SIZE, lif::TILE_SIZE),
			sf::IntRect(lif::TILE_SIZE, 0, lif::TILE_SIZE, lif::TILE_SIZE),
			sf::IntRect(0, 0, lif::TILE_SIZE, lif::TILE_SIZE)
		});
	});
	addComponent<lif::Sounded>(*this,
		lif::sid("warp"), lif::getAsset("test", "teleport.ogg")
	);

	auto& anim = animated->addAnimation("teleport");
	for (unsigned i = 0; i < N_ANIM_FRAMES; ++i)
		anim.addFrame(sf::IntRect(i * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));

	auto& animatedSprite = animated->getSprite();
	animatedSprite.setAnimation(anim);
	animatedSprite.setLooped(true);
	animatedSprite.setFrameTime(sf::seconds(0.15));
	animatedSprite.play();
}

void Teleport::update() {
	lif::Entity::update();
	disableT += lif::time.getDelta();
	if (disabled && disableT >= lif::conf::teleport::COOLDOWN_TIME
			&& collider->getColliding().size() == 0)
	{
		disabled = false;
		animated->getSprite().play();
	}
}

void Teleport::disable() {
	disabled = true;
	animated->getSprite().pause();
	disableT = sf::Time::Zero;
}

void Teleport::_warp(lif::Collider& cld) {
	if (disabled) return;

	const auto& entity = cld.getOwner();
	auto am = entity.get<lif::AxisMoving>();
	if (am != nullptr && !(entity.isAligned() && lif::tile(entity.getPosition()) == lif::tile(position))) {
		return;
	}

	Teleport *nxt = _next;
	for ( ; nxt != nullptr && nxt != this; nxt = nxt->next()) {
		if (nxt->isDisabled() || nxt->get<lif::Collider>()->getColliding().size() > 0)
			continue;
		break;
	}

	if (nxt == nullptr || nxt == this) return;

	cld.getOwnerRW().setPosition(nxt->getPosition());
	if (am != nullptr) {
		am->setPrevAlign(lif::tile(nxt->getPosition()));
	}

	mustSpawnFlash = true;
	nxt->mustSpawnFlash = true;
	lif::cache.playSound(get<lif::Sounded>()->getSoundFile("warp"));
	disable();
	nxt->disable();
}
