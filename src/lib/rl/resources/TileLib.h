#pragma once

#include <map>
#include <string>

#include <oxygine-framework.h>

#include <core/util/json_conversions.h>

enum class eTileCategory : char
{
	// Main categories
	ITEM = 1,
	CREATURE,
	MAP_FEATURE,
	EFFECT,
	ICON
};

namespace pgn
{
	class cTileLib
	{
	public:
		typedef std::map<std::string, oxygine::spSprite> SpriteMap;

		// Load xml file with all tiles and descriptors
		void Init(const std::string& zResName, const std::string& zDescName);
		void Destroy();

		const SpriteMap& GetMap() const { return mSprites; }
		oxygine::spSprite GetSprite(const std::string& name) const;
	private:
		DECL_JSON_PGN_FRIEND

		SpriteMap mSprites;
		oxygine::Resources mTileRes;
	};

	DECL_JSON_PGN(cTileLib)
}