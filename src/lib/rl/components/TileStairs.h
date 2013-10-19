#pragma once

#include <memory>
#include <ecs/ecs_config.h>

namespace pgn
{
	//! Connection to a stairs entity of a different level
	struct cTileStairs
	{
		bool	mDirectionDown;
		cEntity	mConnectedTo;
	};

	// TODO: write event function: ConnectStairs( se1, se2) // Upper to lower
}