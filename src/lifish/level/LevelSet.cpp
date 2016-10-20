#include "LevelSet.hpp"
#include "json.hpp"
#include "utils.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <typeinfo>

using json = nlohmann::json;
using Game::LevelSet;
using Game::Level;

/** Optional additional metadata; all values must be strings */
static constexpr const char* AVAIL_METADATA[] = {
	"author", "created", "difficulty"
};

LevelSet::LevelSet(const std::string& path) {
	json levelJSON = json::parse(std::ifstream(path.c_str()));
	// load metadata
	metadata["name"] = levelJSON["name"].get<std::string>();
	if (metadata["name"].length() < 1)
		metadata["name"] = path;
	for (const auto& key : AVAIL_METADATA) {
		if (levelJSON.find(key) != levelJSON.end())
			metadata[key] = levelJSON[(std::string)key].get<std::string>();
	}

	// load tracks data
	const auto tracksdata = levelJSON["tracks"];
	unsigned tracknum = 1;
	/* trackinfo = {
	 *	"loop": {
	 *		"start": float,
	 *		"end": float
	 *	}
	 * }
	 */
	for (const auto& trackinfo : tracksdata) {
		auto loop = trackinfo["loop"];
		float loopstart = loop["start"]; 
		float looplength = -1;
		auto len = loop.find("length");
		if (len != loop.end()) {
			looplength = *len;
		} else {
			float loopend = loop["end"];
			looplength = loopend - loopstart;
		}
		tracks.push_back(getNthTrack(tracknum++, loopstart, looplength));
	}

	// load enemies data
	const auto enemydata = levelJSON["enemies"];
	unsigned enemynum = 0;
	/* enemyinfo = {
	 *	"ai": uint,
	 *	"speed": uint,
	 *	"attack": {
	 *		"type": string,
	 *		"damage": int,
	 *		"id": uint         [opt]
	 *		"speed": float,      [opt]
	 *		"fireRate": float,   [opt]
	 *		"blockTime": float,  [opt]
	 *		"tileRange": int,  [opt, default=-1]
	 *		"pixelRange": float, [opt, only valid if tileRange is not specified]
	 *	}
	 *
	 * }
	 */
	for (const auto& enemyinfo : enemydata) {
		enemies[enemynum].ai = enemyinfo["ai"];
		enemies[enemynum].speed = enemyinfo["speed"];
		
		auto atk = enemyinfo["attack"];
		auto atktype = atk["type"];
		
		enemies[enemynum].attack.type = Game::AttackType::AXIS_BOUND;
		for (unsigned i = 0; i < atktype.size(); ++i) {
			AttackType type;
			auto at = atktype[i].get<std::string>();
			if (!Game::stringToAttackType(at, type))
				throw std::invalid_argument(at.c_str());

			enemies[enemynum].attack.type = AttackType(
					static_cast<unsigned>(enemies[enemynum].attack.type) 
					| static_cast<unsigned>(type));
		}

		// Mandatory fields
		enemies[enemynum].attack.damage = atk["damage"].get<int>();
		// Optional fields
		auto it = atk.find("speed");
		if (it != atk.end())
			enemies[enemynum].attack.speed = it->get<float>();
		
		it = atk.find("id");
		if (it != atk.end())
			enemies[enemynum].attack.id = it->get<unsigned>();

		it = atk.find("fireRate");
		if (it != atk.end())
			enemies[enemynum].attack.fireRate = it->get<float>();

		it = atk.find("blockTime");
		if (it != atk.end())
			enemies[enemynum].attack.blockTime = sf::milliseconds(it->get<float>());

		bool range_found = false;
		enemies[enemynum].attack.rangeInTiles = true;
		it = atk.find("tileRange");
		if (it != atk.end()) {
			enemies[enemynum].attack.tileRange = it->get<int>();
			range_found = true;
		}
		if (!range_found) {
			it = atk.find("pixelRange");
			if (it != atk.end()) {
				enemies[enemynum].attack.pixelRange = it->get<float>();
				enemies[enemynum].attack.rangeInTiles = false;
				range_found = true;
			}
		}
		if (!range_found) {
			enemies[enemynum].attack.tileRange = -1;
			enemies[enemynum].attack.pixelRange = -1;
		}
		++enemynum;
	}


	// load levels data
	const auto levelsdata = levelJSON["levels"];

	unsigned lvnum = 1;
	/* lvinfo = {
	 *	"time": uint,
	 *	"tilemap": string,
	 *	"music": uint,
	 *	"tileIDs": {
	 *		"border": uint,
	 *		"bg": uint,
	 *		"fixed": uint,
	 *		"breakable": uint
	 *	}
	 * }
	 */
	for (const auto& lvinfo : levelsdata) {
		LevelInfo info;
		info.levelnum          = lvnum++;
		info.time              = lvinfo["time"];
		info.track             = tracks[lvinfo["music"].get<unsigned>()-1];
		info.tilemap           = lvinfo["tilemap"];
		info.tileIDs.border    = lvinfo["tileIDs"]["border"];
		info.tileIDs.bg        = lvinfo["tileIDs"]["bg"];
		info.tileIDs.fixed     = lvinfo["tileIDs"]["fixed"];
		info.tileIDs.breakable = lvinfo["tileIDs"]["breakable"];
		levels.push_back(info);
	}
}

std::unique_ptr<Level> LevelSet::getLevel(unsigned num) const {
	std::unique_ptr<Level> level;
	if (num > 0 && num <= levels.size()) {
		level = std::unique_ptr<Level>(new Level(*this));
		level->levelInfo = levels[num - 1];
		if (!level->init()) {
			level.reset();
		}
	}
	return level;
}

std::string LevelSet::toString() const {
	std::stringstream ss;
	ss << "Level Set: " << metadata.find("name")->second << "\r\n"
	   << "Tracks: " << tracks.size() << "\r\n"
	   << "Levels: " << levels.size() << "\r\n";
	for (const auto& pair : metadata) {
		if (pair.first == "name") continue;
		ss << pair.first << ": " << pair.second << "\r\n";
	}
	return ss.str();
}
