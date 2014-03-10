#pragma once

#include <core/container/dict.h>
#include <ecs/ecs.h>

namespace pgn
{
	//! TODO: add/init with actor and world, as they are frequently used entities
	struct cBlackBoard
	{
		//! Persistent general-purpose data
		cDict mDictPerm;

		//! Single-round general-purpose data
		cDict mDictTemp;
	};
}