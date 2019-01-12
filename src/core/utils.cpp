#include <SFML/Audio/Music.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include "utils.hpp"
#include "Options.hpp"
#include "json.hpp"

using json = nlohmann::json;

void sf::to_json(json& j, const sf::Time& time) {
	j = time.asMilliseconds();
}

void sf::from_json(const json& j, sf::Time& time) {
	time = sf::milliseconds(j);
}

void lif::testMusic() {
	sf::Music sample;
	sample.openFromFile(lif::getAsset("music", "music1.ogg"));
	sample.play();
	sample.setVolume(lif::options.musicVolume);
	sf::sleep(sf::milliseconds(200));
	sample.stop();
}
