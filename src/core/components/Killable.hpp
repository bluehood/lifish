#pragma once

#include <functional>
#include "Component.hpp"

namespace Game {

class Killable : public Game::Component {
protected:
	using OnKillCallback = std::function<void()>;
	using CheckKillCallback = std::function<bool()>;

	bool killed = false;
	sf::Clock deathClock;

	/** Function to be called on kill() */
	OnKillCallback onKill;
	CheckKillCallback checkKillProgress;

public:
	ENTITY_ID("Killable")

	explicit Killable(Game::Entity& owner); 
	explicit Killable(Game::Entity& owner, OnKillCallback callback); 
	explicit Killable(Game::Entity& owner, OnKillCallback callback, CheckKillCallback checkKill); 

	void kill();
	void resurrect() { killed = false; }
	bool isKilled() const { return killed; }
	/** 
	 * If checkKill callback is provided, this will return true if the kill is still
	 * happening and therefore the owner should not be disposed of.
	 * If not provided, will always return false.
	 * Note that isKillInProgress() implies isKilled()
	 */ 
	bool isKillInProgress() const;

	/** @return The time passed since kill() was called last. Only valid if `isKilled()`. */
	sf::Time timeSinceDeath() const { return deathClock.getElapsedTime(); }
};

}
