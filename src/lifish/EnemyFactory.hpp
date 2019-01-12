#pragma once

#include <SFML/System.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

#include "Enemy.hpp"

namespace lif {

class LevelManager;
class Enemy;

class EnemyFactory final : private sf::NonCopyable {
public:
	static std::unique_ptr<lif::Enemy> create(const lif::LevelManager& ls, unsigned id, const sf::Vector2f& pos);
};

}
