#pragma once

#include <map>
#include <oxygine-framework.h>

enum class eTileCategory : char
{
	// Main categories
	ITEM=1,
	CREATURE,
	MAP_FEATURE,
	EFFECT,
	ICON
};

class cTileLib
{
public:
	typedef std::map<std::string, oxygine::spSprite> SpriteMap;

	// Load xml file with all tiles and descriptors
	void Init(oxygine::Resources& res);

	SpriteMap& GetMap() { return mSprites; }
private:
	SpriteMap mSprites;
};