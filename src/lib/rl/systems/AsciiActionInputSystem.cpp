#pragma once

#include "AsciiActionInputSystem.h"

namespace pgn
{
	//------------------------------------------------------
	void cAsciiActionInputSystem::Process()
	{
		// TODO: simple turn taking at the moment.
		// Move both entities to a random position
	}

	//------------------------------------------------------
	bool cAsciiActionInputSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}