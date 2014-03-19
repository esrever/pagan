#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>

#include <rlut/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cTeleport : public cBase
			{
				public:
					bool operator()(ecs::cEntityWithData&, const ecs::cmp::cLocation& loc);
			};
		}
	}
}