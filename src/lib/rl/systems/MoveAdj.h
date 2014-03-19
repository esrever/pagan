#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cMoveAdj : public cBase
			{
				public:
					bool operator()(ecs::cEntityWithData&, const glm::ivec2& dir);
			};
		}
	}
}