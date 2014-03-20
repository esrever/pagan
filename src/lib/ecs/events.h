#pragma once

#include <core/event/Event.h>
#include <ecs/ecs.h>

namespace pgn
{
	namespace evt
	{
		DECL_EVT1(EntityCreated, ecs::cEntityWithData);
		DECL_EVT1(EntityDestroy, ecs::cEntityWithData);
	}
}