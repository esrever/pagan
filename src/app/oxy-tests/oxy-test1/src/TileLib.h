#pragma once

#include <cstdint.h>

enum class eTileCategory : std::uint8_t
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
	// Load xml file with all tiles and descriptors
	void Init(const char * zFileName);
private:
	std::vector<oxygine::spResAnim> mTileMaps;
};