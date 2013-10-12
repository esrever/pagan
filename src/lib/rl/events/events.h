#pragma once

#include <ecs/ecs_config.h>

namespace pgn
{
	//! A specific tile in the world has been modified
	DECL_EVENT(TileModified, cEntityWptr, Location);
}