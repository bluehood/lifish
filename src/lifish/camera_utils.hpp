#pragma once

#include <SFML/System/Time.hpp>

namespace sf {
class Time;
}  // namespace sf

namespace lif {

void requestCameraShake(float xAmplitude, float xFrequency, float yAmplitude, float yFrequency,
		sf::Time duration, float fadeFactor);

}
