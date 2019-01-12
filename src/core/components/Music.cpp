#include <SFML/System/Time.hpp>
#include <iostream>
#include <string>

#include "Music.hpp"
#include "LoopingMusic.hpp"

using lif::Music;

Music::Music(lif::Entity& owner, const lif::Track& track)
	: lif::Component(owner)
	, track(track)
{
	_declComponent<Music>();
	if (!musicInput.openFromFile(track.name)) {
		std::cerr << "Error: couldn't load music " << track.name << " from file!" << std::endl;
	} else {
		music = std::make_unique<LoopingMusic>(musicInput);
		music->setLoopPoints(sf::seconds(track.loopstart), sf::seconds(track.loopend));
		music->setLoop(true);
	}
}
