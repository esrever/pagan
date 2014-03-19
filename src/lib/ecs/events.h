#pragma once

#include <ecs/ecs.h>

namespace pgn
{
	namespace evt
	{
		DECL_EVT1(EntityCreated, ecs::cEntityWithData);
		DECL_EVT1(EntityDestroy, ecs::cEntityWithData);
	}
}