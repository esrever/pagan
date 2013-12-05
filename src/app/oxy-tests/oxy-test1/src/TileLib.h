#pragma once

#include <map>
#include <string>

#include <oxygine-framework.h>

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