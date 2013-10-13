#pragma once

#include <ecs/ecs_config.h>
#include <rl/components/components.h>

namespace pgn
{
	//! A specific tile in the world has been modified
	DECL_EVENT2(TileInLevelChanged, cEntityWptr, cLocation);
	//! A level has been created
	DECL_EVENT1(LevelCreated, cEntityWptr);
	//! A level is about to be destroyed
	DECL_EVENT1(LevelDestroy, cEntityWptr);
}