#pragma once

#include <memory>

namespace pgn
{
	class cEntity;

	//! Connection to a stairs entity of a different level
	struct cTileStairs
	{
		bool					mDirectionDown;
		std::weak_ptr<cEntity>	mConnectedTo;
	};

	// TODO: write event function: ConnectStairs( se1, se2) // Upper to lower
}