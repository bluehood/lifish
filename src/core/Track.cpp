#include "Track.hpp"
#include "core.hpp"

Game::Track Game::getNthTrack(unsigned num, float start, float length) {
	using Game::DIRSEP;
	std::stringstream ss;
	ss << Game::pwd << DIRSEP << "assets" << DIRSEP << "music" << DIRSEP << "music" << num << ".ogg";
	Game::Track track;
	track.name = ss.str();
	track.loopstart = start;
	track.looplength = length;
	track.loopend = start + length;
	return track;
}
