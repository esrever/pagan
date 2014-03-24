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
			class cCreateLevel : public cBase
			{
				public:
					cCreateLevel();
					bool operator()(ecs::cArchetypeWithData&);

				private:
			};
		}
	}
}