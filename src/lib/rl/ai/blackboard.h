#pragma once

#include <core/container/dict.h>
#include <ecs/ecs.h>

namespace pgn
{
	struct cBlackBoard
	{
		//! Persistent general-purpose data
		cDict mDictPerm;

		//! Single-round general-purpose data
		cDict mDictTemp;

		//! TODO: add/init frequently used entities
		/*
			entity ThisActor
			entity World
			entity ActorLevel
		*/
	};
}