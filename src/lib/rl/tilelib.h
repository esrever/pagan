#pragma once

#include <string>
#include <unordered_map>

#include <core/util/jsonutils.h>

#include "tile.h"

class cTileLib
{
	public:
		void Init(const char * jsonfname);
	private:
		std::unordered_map<std::string, cTile> mTilesMap;
};