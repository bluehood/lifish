#pragma once

#include <string>
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/NonCopyable.hpp>
#include "game.hpp"
#include "Entity.hpp"
#include "Track.hpp"
#include "EntityType.hpp"
#include "utils.hpp"

namespace Game {

class LevelSet;
class LevelNumText;

/** Holds the data used in Level initialization. */
struct LevelInfo {
	struct {
		unsigned bg        = 1,
			 border    = 1,
			 fixed     = 1,
			 breakable = 1;
	} tileIDs;
	
	/** This game's track info */
	Game::Track track;

	/** Number of the level */
	unsigned levelnum = 0;

	/** Time before "Hurry Up" (in seconds) */
	unsigned time = 0;

	/** String representation of the tilemap */
	std::string tilemap;
};

/**
 * The Level class contains the template of a level, with all the
 * static information about it. Dynamic informations about the level
 * during the game are managed by Game::LevelManager.
 */
class Level final 
	: public Game::Entity
	, public sf::Drawable
	, private sf::NonCopyable
{
	friend class Game::LevelSet;

	/** Types of bg texture tiles */
	enum : unsigned {
		TILE_REGULAR     = 0,
		TILE_UPPER_LEFT  = 1,
		TILE_UPPER_RIGHT = 2,
		TILE_LOWER_LEFT  = 3,
		TILE_LOWER_RIGHT = 4,
		TILE_UPPER       = 5,
		TILE_LOWER       = 6,
		TILE_LEFT        = 7,
		TILE_RIGHT       = 8,
	};

	/** This ought to be set before calling level.init(); */
	Game::LevelInfo levelInfo;

	////////////////////////// COMPONENTS ////////////////////////////

	/** The background texture */
	sf::Texture *bgTexture = nullptr;

	/** The borders' texture */
	sf::Texture *borderTexture = nullptr;
	
	/** The text containing the level number */
	Game::LevelNumText *levelnumtext = nullptr;
	
	///////////////////////////////////////////////////////////////////

	/** This level's static (initial) tilemap */
	Game::Matrix<Game::EntityType, LEVEL_HEIGHT, LEVEL_WIDTH> tiles;

	/** The sprites for the background tiles (8 border + background) */
	std::array<sf::Sprite, 9> bgTiles;
	std::array<sf::Texture, 9> bgTextures;

	/** The LevelSet this level belongs to */
	const Game::LevelSet& levelSet;

	/** Whether this level has been initialized or not */
	bool initialized = false;


	/** Loads the background/border textures from the file (or from memory, if
	 *  texture has already been cached)
	 */
	void _loadTextures();

	/** Loads the content of bgTiles (bgTexture must already be set) */
	void _loadTiles();

	/** Given a string representation of the level, sets its static tilemap
	 *  by filling the `entities` vector.
	 */
	bool _setTilemap(const std::string& tilemap);

public:
	/** Constructs a level without a specified time and tileset. init() must
	 *  be called before using this level. You need to specify a LevelSet
	 *  this Level belongs to.
	 */
	Level(const LevelSet& levelSet);

	/** Loads the appropriate bgTexture, fills the bgTiles and makes this level
	 *  usable. Must be called after setting levelInfo.
	 *  Returns nullptr if there were errors, self otherwise.
	 */
	Game::Entity* init() override;
	bool isInitialized() const { return initialized; }

	const LevelInfo& getInfo() const { return levelInfo; }

	void setTime(const unsigned _time) { levelInfo.time = _time; }

	/** Gets tile[left][top] */
	EntityType getTile(unsigned left, unsigned top) const;

	/** Changes the origin of all tiles */
	void setOrigin(const sf::Vector2f& origin) override;

	/** Draws this level's background in the target window */
	void draw(sf::RenderTarget& window, sf::RenderStates states) const override;

	const Game::LevelSet& getLevelSet() const { return levelSet; }

	std::string toString() const override;
	std::string getTilemap() const;
};

}
