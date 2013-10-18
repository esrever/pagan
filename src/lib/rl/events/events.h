#pragma once

#include <ecs/ecs_config.h>
#include <ecs/Entity.h>
#include <rl/components/components.h>

namespace pgn
{
	//! A specific tile in the world has been modified -- <NewTileInstance, Location>
	DECL_EVENT2(TileInLevelChanged, cEntityWptr, cLocation);
	//! A level has been created -- <Level>
	DECL_EVENT1(LevelCreated, cEntityWptr);
	//! A level is about to be destroyed -- <Level>
	DECL_EVENT1(LevelDestroy, cEntityWptr);

	//! Apply an action to an adjacent tile
	DECL_EVENT2(MoveToAdjacent, cEntityWptr, cLocation);

	DECL_EVENT0(ExitApplication);
}