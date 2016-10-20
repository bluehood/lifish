#pragma once

#include "Screen.hpp"
#include "Action.hpp"

namespace Game {

class ShadedText;

namespace UI {

class PreferencesScreen : public Game::UI::Screen {
	enum class VolumeType { MUSIC, SOUND };
	enum class VolumeAction { RAISE, LOWER, MUTE_TOGGLE };

	constexpr static unsigned MAX_VOLUME = 15;
	constexpr static unsigned SPEAKER_SPRITE_SIZE = 25;

	/** Hook for the music volume bar (used in draw()) */
	Game::ShadedText *musicVolumeBar = nullptr;
	/** Hook for the sounds volume bar (used in draw()) */
	Game::ShadedText *soundsVolumeBar = nullptr;

	int relMusicVolume = MAX_VOLUME,
	      relSoundVolume = MAX_VOLUME;

	/** Used to toggle music muteness */
	float prevMusicVolume = -1;
	sf::Texture *speakerTexture = nullptr;

	Game::UI::Action _changeVolume(VolumeType which, VolumeAction what);
public:
	explicit PreferencesScreen(const sf::RenderWindow& window, const sf::Vector2u& size);
};

}

}
