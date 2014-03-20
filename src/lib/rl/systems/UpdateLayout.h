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
			class cUpdateLayout : public cBase
			{
				public:
					bool operator()(ecs::cEntityWithData& ed, cmp::cLocation * zOldLoc);
			};
		}
	}
}