#pragma once

#include "GameTurnSystem.h"

namespace pgn
{
	//------------------------------------------------------
	void cGameTurnSystem::Process()
	{
		// TODO: simple turn taking at the moment.
		// Move both entities to a random position
	}

	//------------------------------------------------------
	bool cGameTurnSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}