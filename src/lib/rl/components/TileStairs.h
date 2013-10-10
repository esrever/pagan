#pragma once

namespace pgn
{
	struct cEntity;

	//! Connection to a stairs entity of a different level
	struct cStairs
	{
		bool	mDirectionDown;
		cEntity	mConnectedTo;
	};

	// TODO: write event function: ConnectStairs( se1, se2) // Upper to lower
}