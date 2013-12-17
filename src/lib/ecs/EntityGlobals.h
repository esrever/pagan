#pragma once

#include "Entity.h"
#include "EntityData.h"

namespace pgn
{
	struct cEntityGlobals
	{
		cEntityWithData mPC;
		cEntityWithData mWorld;
		cEntityWithData mMapWindow;
		cEntityWithData mStatusWindow;
	};
}