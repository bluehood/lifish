#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Angle.hpp"
#include "Entity.hpp"

namespace lif {

class Sprite;
class Angle;

class SurgeWarn : public lif::Entity {

	const sf::Time duration;

	lif::Sprite *sprite = nullptr;

	sf::Time t = sf::Time::Zero;

public:
	explicit SurgeWarn(const sf::Vector2f& pos, const sf::Time& duration, const lif::Angle& rotation);

	void update() override;
};

}
