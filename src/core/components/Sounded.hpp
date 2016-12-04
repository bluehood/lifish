#pragma once

#include <string>
#include <vector>
#include "Component.hpp"

/**
 * An object which has one or more sounds associated with it. 
 */
namespace Game {

class Sounded : public Game::Component {
	std::vector<std::string> soundFiles;
public:
	ENTITY_ID("Sounded")
	explicit Sounded(Game::Entity& owner, std::initializer_list<std::string> _soundFiles) 
		: Game::Component(owner)
	{
		for (auto& s : _soundFiles)
			soundFiles.push_back(s);
	}

	std::string getSoundFile(unsigned short n = 0) const { 
		return n >= soundFiles.size() ? "" : soundFiles[n];
	}
};

}
