#pragma once

#include "GameEntity.h"

/*
	I need the standard read_json, but I want an additional custom version. 
	Perhaps here I call both the one for the base class and then my specific
*/

class cTile : public cGameEntity
{
	friend void read_json(cGameEntity& obj, const rapidjson::Document& doc);
	public:
	protected:
		bool mPass;
};

template<>
void read_json<cTile>(cTile& obj, const rapidjson::Document& doc);