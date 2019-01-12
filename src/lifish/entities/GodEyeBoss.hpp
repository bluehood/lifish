#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Boss.hpp"

namespace sf {
class RenderTarget;
}  // namespace sf

namespace lif {

class Animated;
class FreeSighted;
class HurtDrawProxy;
class LevelManager;

class GodEyeBoss : public lif::Boss, public sf::Drawable {

	enum LevelConfiguration {
		BOSS_RIGHT = 0,
		BOSS_LEFT,
		BOSS_TOP,
		BOSS_BOTTOM
	} lvConfiguration = BOSS_RIGHT;

	lif::LevelManager& lm;

	lif::Animated *spriteBg = nullptr,
	              *spriteFg = nullptr,
	              *shield = nullptr;
	lif::HurtDrawProxy *hurtDrawProxy = nullptr;
	lif::FreeSighted *sighted = nullptr;

	sf::Time shieldAlphaT;

	int timesHurt = 0;

	void _updatePupilPos();
	void _updateShield();
	/** Main post-hurt routine */
	void _onHurt();
	void _teleportTo(const sf::Vector2f& newpos);
	/** Shuffles the level's wall configuration and updates `lvConfiguration` accordingly. */
	void _shakeWalls();
	void _kill() override;
public:
	explicit GodEyeBoss(const sf::Vector2f& pos, lif::LevelManager& lm);

	void update() override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}
