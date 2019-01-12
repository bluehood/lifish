#pragma once

/** General-purpose game setup */
#include <SFML/System.hpp>
#include <stddef.h>
#include <chrono>
#include <random>
#include <sstream>
#include <string>

#include "Options.hpp"
#include "Time.hpp"

// Fallback in case the game wasn't compiled properly with cmake
#ifndef VERSION
#	define VERSION "unknown"
#endif
#ifndef COMMIT
#	define COMMIT "unknown"
#endif

namespace lif {

struct Options;
class MusicManager;
class GameCache;
#ifdef MULTITHREADED
class WindowContext;
#endif
template<typename ClockType>
class Time;

/****************************************************************************/
/*                         GLOBAL DEFINITIONS                               */
/****************************************************************************/

#ifdef SFML_SYSTEM_WINDOWS
constexpr char DIRSEP = '\\';
#else
constexpr char DIRSEP = '/';
#endif

constexpr int TILE_SIZE = 32; // pixels

constexpr auto PI = 3.141592653589793238L;
constexpr size_t PWD_BUFSIZE = 512;

/** Threshold value to consider an input from joystick getAxisPosition(). */
constexpr auto JOYSTICK_INPUT_THRESHOLD = 50;

/****************************************************************************/
/*                         GLOBAL VARIABLES                                 */
/****************************************************************************/

/** The executable working directory */
extern char pwd[PWD_BUFSIZE];

/** The assets base directory */
extern std::string assetDir;

/** Global game cache (caches textures in memory for faster loading) */
extern lif::GameCache cache;

/** Random number generator */
extern std::default_random_engine rng;

/** The game options */
extern lif::Options options;

/** The time manager */
extern lif::Time<std::chrono::high_resolution_clock> time;

/** Pointer to an unowned MusicManager, which *MUST* be created in the main function.
 *  Allows easy access to the music manager throughout the code, but since the object
 *  itself is created in the main, it has automatic storage and is guaranteed to die before the program's exit.
 */
extern lif::MusicManager *musicManager;

#ifndef RELEASE
class DebugPainter;

/** See musicManager */
extern lif::DebugPainter *debugPainter;
#endif

#ifdef MULTITHREADED
/** Pointer to the current WindowContext (unowned), used by the rendering thread
 *  to draw the correct context with no hassle.
 */
extern lif::WindowContext *curContext;
#endif

/** If true, the game exits after the current loop */
extern bool terminated;
extern int exitCode;

/****************************************************************************/
/*                         GLOBAL FUNCTIONS                                 */
/****************************************************************************/

constexpr const std::string& getAssetDir() {
	return assetDir;
}

inline void _getAssetInternal(std::stringstream&) {}

template<typename... Args>
inline void _getAssetInternal(std::stringstream& ss, const std::string& first, Args&&... rest) {
	ss << first << DIRSEP;
	_getAssetInternal(ss, rest...);
}

/** Returns the asset found under assetDir/{path args joined by DIRSEP} */
template<typename ...Args>
inline std::string getAsset(Args&&... path) {
	std::stringstream ss;
	ss << getAssetDir();
	_getAssetInternal(ss, path...);
	auto s = ss.str();
	s.resize(s.length() - 1);
	return s;
}

/** Initializes runtime variables */
bool initCore();

}
