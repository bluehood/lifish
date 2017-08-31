#include "Lifed.hpp"
#include <sstream>

using lif::Lifed;

Lifed::Lifed(lif::Entity& owner, const int life)
	: lif::Component(owner)
	, maxLife(life)
	, life(life)
{
	_declComponent<Lifed>();
}

Lifed::Lifed(lif::Entity& owner, const int life, OnHurtCallback _onHurt)
	: lif::Lifed(owner, life)
{
	onHurt = _onHurt;
}

std::string Lifed::toString() const {
	std::stringstream ss;
	ss << "Life: " << life << " / " << maxLife;
	return ss.str();
}

int Lifed::decLife(int dam, bool triggerCallback) {
	life -= dam > life ? life : dam;

	if (triggerCallback && onHurt)
		onHurt(dam, life);

	return life;
}
