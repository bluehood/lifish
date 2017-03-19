#pragma once

#include "Screen.hpp"
#include "Action.hpp"

namespace lif {

class ShadedText;

namespace ui {

class PreferencesScreen : public lif::ui::Screen {
	enum class VolumeType { MUSIC, SOUND };
	enum class VolumeAction { RAISE, LOWER, MUTE_TOGGLE };

	constexpr static unsigned MAX_VOLUME = 15;
	constexpr static unsigned SPEAKER_SPRITE_SIZE = 25;

	/** Hook for the music volume bar (used in draw()) */
	lif::ShadedText *musicVolumeBar = nullptr;
	/** Hook for the sounds volume bar (used in draw()) */
	lif::ShadedText *soundsVolumeBar = nullptr;

	short relMusicVolume = MAX_VOLUME,
	      relSoundVolume = MAX_VOLUME;

	/** Used to toggle music muteness */
	float prevMusicVolume = -1;
	sf::Texture *speakerTexture = nullptr;

	lif::ui::Action _changeVolume(VolumeType which, VolumeAction what);
	void _setupCallbacks();
	void _setupTransitions();
public:
	explicit PreferencesScreen(const sf::RenderWindow& window, const sf::Vector2u& size);
};

}

}
