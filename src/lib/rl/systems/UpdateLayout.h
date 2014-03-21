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
			// Updates layout data when entities move
			class cUpdateLayout : public cBase
			{
				public:
					bool operator()(ecs::cEntityWithData& ed, cmp::cLocation * zOldLoc);
				private:
					void AddToLayout(ecs::cEntityWithData& ed, rl::cLayout * lay);
					void RemoveFromLayout(ecs::cEntityWithData& ed, rl::cLayout * lay);
					void UpdateLayout(ecs::cEntityWithData& ed, rl::cLayout * lay);
			};
		}
	}
}