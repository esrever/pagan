#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>

#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cTeleport : public cBase
			{
				public:
					//! sometimes teleporting is instant, sometimes not.
					bool operator()(ecs::cEntityWithData&, const ecs::cmp::cLocation& loc, float tu = 1.0f);
			};
		}
	}
}