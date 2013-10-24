#pragma once

#include "GameTurnSystem.h"

namespace pgn
{
	//------------------------------------------------------
	void cGameTurnSystem::Process()
	{

	}

	//------------------------------------------------------
	bool cGameTurnSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}