#pragma once

#include "Component.hpp"

namespace Game {

/**
 * An object with a life. Life cannot be < 0.
 */
class Lifed : public Game::Component {
protected:
	int life;
	const int maxLife;
public:
	explicit Lifed(Game::Entity& owner, const int life)
		: Game::Component(owner)
		, life(life) 
		, maxLife(life) 
	{
		if (life < 0)
			throw std::invalid_argument("Lifed: maxLife cannot be smaller than 0!");
	}

	bool isAlive() const { return life > 0; }
	int getLife() const { return life; }
	int getMaxLife() const { return maxLife; }
	int decLife(int dam) { life -= dam > life ? life : dam; return life; }
	void setLife(int lf) { life = lf < 0 ? 0 : lf; }
	void refillLife() { life = maxLife; }
};

}
