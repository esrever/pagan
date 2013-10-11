#pragma once

namespace pgn
{
	//! Owned by entities that can be locked/unlocked
	struct cLockable
	{
		float mDifficulty;
	};

	//! TODO: events: lock, unlock. kick, bash are for the breakable component?
}