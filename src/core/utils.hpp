#pragma once

#include <array>
#include <ostream>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include "game.hpp"

namespace Game {

template<class T, size_t ROWS, size_t COLS>
using Matrix = std::array<std::array<T, COLS>, ROWS>;

/// Polyfill ///
template<typename T> 
constexpr T abs(T num) {
#ifdef IS_APPLE
	// Apple Clang is a "good" compiler...
	if (num < 0) return -num;
	return num;
#else
	return std::abs(num);
#endif
}

template<typename T>
inline std::string to_string(T val) {
#ifdef __MINGW32__
	std::stringstream ss;
	ss << val;
	return ss.str();
#else
	return std::to_string(val);
#endif
}
//////////////

/** Returns the aligned tile corresponding to position `pos` by
 *  flooring its components and dividing by TILE_SIZE.
 */
inline sf::Vector2i tile(const sf::Vector2f& pos) {
	return sf::Vector2i(pos.x / Game::TILE_SIZE, pos.y / Game::TILE_SIZE);
}

/** Returns the pixel position `pos` aligned to the nearest tile. */
inline sf::Vector2f aligned(const sf::Vector2f& pos) {
	return sf::Vector2f(
		(int(pos.x)/TILE_SIZE) * TILE_SIZE,
		(int(pos.y)/TILE_SIZE) * TILE_SIZE);
}

template<typename T>
constexpr std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& vec) {
	return stream << "(" << vec.x << ", " << vec.y << ")";
}

template<typename T>
constexpr std::ostream& operator<<(std::ostream& stream, const sf::Rect<T>& rect) {
	return stream << "(" << rect.left << ", " << rect.top 
		<< " x " << rect.width << ", " << rect.height << ")";
}

template<typename T, typename R>
constexpr double sqrDistance(const sf::Vector2<T>& a, const sf::Vector2<R>& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

constexpr int manhattanDistance(const sf::Vector2i& tileA, const sf::Vector2i& tileB) {
	return Game::abs(tileA.x - tileB.x) + Game::abs(tileA.y - tileB.y);
}

constexpr float centerX(const sf::FloatRect& bounds, const sf::FloatRect& rect) {
	return rect.left + (rect.width - bounds.width) / 2.;
}

constexpr float centerY(const sf::FloatRect& bounds, const sf::FloatRect& rect) {
	return rect.top + (rect.height - bounds.height) / 2.;
}

/** Given the bounding box of something, returns the coordinates
 *  which center that thing relatively to the given rectangle.
 *  bounds = object to center
 *  rect = container to center the object within (default: main game window)
 */
inline sf::Vector2f center(const sf::FloatRect& bounds, const sf::FloatRect& rect = 
		sf::FloatRect(Game::MAIN_WINDOW_SHIFT, 0, 
			Game::LEVEL_WIDTH * Game::TILE_SIZE,
			Game::LEVEL_HEIGHT * Game::TILE_SIZE))
{
	return sf::Vector2f(centerX(bounds, rect), centerY(bounds, rect));
}

inline bool startsWith(const std::string& haystack, const std::string& needle) {
	return haystack.substr(0, needle.size()) == needle;
}

/** Plays a brief sound */
void testMusic();

/** If Game::options.showFPS == true, then display FPS on screen */
void maybeShowFPS(sf::RenderWindow& window);

namespace KeyUtils {
	int keyToNumber(sf::Keyboard::Key key);

	std::string keyToString(sf::Keyboard::Key key);
}

} // end namespace Game
