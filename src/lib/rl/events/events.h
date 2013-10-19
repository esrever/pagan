#pragma once

#include <ecs/ecs_config.h>
#include <ecs/Entity.h>
#include <ecs/EntityMgr.h>
#include <rl/components/components.h>

namespace pgn
{
	//! A specific tile in the world has been modified -- <NewTileInstance, Location>
	DECL_EVENT2(TileInLevelChanged, cEntityWithComponents, cLocation);
	//! A level has been created -- <Level>
	DECL_EVENT1(LevelCreated, cEntityWithComponents);
	//! A level is about to be destroyed -- <Level>
	DECL_EVENT1(LevelDestroy, cEntityWithComponents);

	//! Apply an action to an adjacent tile
	DECL_EVENT2(MoveToAdjacent, cEntityWithComponents, cLocation);

	DECL_EVENT0(ExitApplication);
}