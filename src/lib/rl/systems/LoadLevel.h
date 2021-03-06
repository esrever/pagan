#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <rl/event/events.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//! What happens when we load a level, e.g. fill the layout, regen the seeds, apply diffs, etc
			class cLoadLevel : public cBase
			{
				public:
					cLoadLevel();
					bool operator()(ecs::cEntityWithData&);

				private:
			};
		}
	}
}