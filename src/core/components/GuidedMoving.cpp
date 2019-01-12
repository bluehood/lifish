#include <SFML/System/Vector2.inl>
#include <algorithm>
#include <numeric>

#include "GuidedMoving.hpp"
#include "Time.hpp"
#include "utils.hpp"
#include "Entity.hpp"
#include "core.hpp"

using lif::GuidedMoving;

GuidedMoving::GuidedMoving(lif::Entity& owner,
		const sf::Vector2f& start, const sf::Vector2f& end, sf::Time timeTaken,
		std::initializer_list<lif::GuidedMoving::ModFunc> modfuncs)
	: lif::Moving(owner, 0)
	, start(start)
	, end(end)
	, timeTaken(timeTaken)
	, modfuncs(modfuncs)
	, tPerc(0)
{
	_declComponent<GuidedMoving>();

	_updatePosition();
}

GuidedMoving::GuidedMoving(lif::Entity& owner,
		const sf::Vector2f& start, const sf::Vector2f& end, sf::Time timeTaken,
		GuidedMoving::ModFunc modfunc)
	: GuidedMoving(owner, start, end, timeTaken, { modfunc })
{}

void GuidedMoving::update() {
	lif::Component::update();

	// Calculate this once here
	tPerc += lif::time.getDelta() / timeTaken;

	_updatePosition();
}

void GuidedMoving::_updatePosition() {
	owner.setPosition(std::accumulate(modfuncs.begin(), modfuncs.end(), _calcPathPos(tPerc),
		[this] (const auto& pos, const auto& pair)
	{
		return pos + this->_calcModFunc(
				std::get<0>(pair),
				std::get<1>(pair) ? tPerc : std::min(1.0f, tPerc),
				std::get<2>(pair));
	}));
}

sf::Vector2f GuidedMoving::_calcPathPos(float perc) const {
	return perc > 1 ? end : start + (end - start) * perc;
}

sf::Vector2f GuidedMoving::_calcModFunc(const GuidedMoving::_ModFunc& f, float perc, bool useRelative) const {
	const auto v = f(perc);

	if (!useRelative)
		return v;

	// Angle between (end - start) and x axis
	const auto angle = lif::normalized(end - start).x;
	const float c = std::cos(angle),
	            s = std::sin(angle);

	// Tilt the resulting vector to use (end - start) as the horizontal axis
	return sf::Vector2f(v.x * c - v.y * s, v.x * s + v.y * c);
}

void GuidedMoving::reset() {
	tPerc = 0;
}

bool GuidedMoving::isAtEnd() const {
	return tPerc > 1;
}
