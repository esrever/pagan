#pragma once

#include <map>
#include <string>

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
	void Init(oxygine::ResAnim * tilemap);

	const SpriteMap& GetMap() { return mSprites; }
	oxygine::spSprite GetSprite(const std::string& name);
private:
	SpriteMap mSprites;
};