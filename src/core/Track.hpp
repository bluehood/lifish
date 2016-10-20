#pragma once

#include <string>
#include <sstream>

namespace Game {

/**
 * Struct containing a track's data
 */
struct Track {
	std::string name;
	float loopstart, loopend, looplength;
};

Track getNthTrack(unsigned num, float start, float length);

}
