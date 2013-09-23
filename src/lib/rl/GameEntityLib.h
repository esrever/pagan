#pragma once

#include <string>
#include <unordered_map>

#include <core/util/jsonutils.h>

#include "GameEntity.h"

/*
	Do I use a templated get as in getComponent?
	Do i have a separaate abstract hier for others, nonascii, events, etc?
	Make clear names, GameEntity perhaps not great.Gfx component?
*/

class cGameEntityLib
{
	public:
		void Init(const char * tilesFile,
				  const char * creaturesFile);
	private:
		//std::unordered_map<std::string, cTile> mTilesMap;
};