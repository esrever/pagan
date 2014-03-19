#include "actions.h"

#include <core/app/sdlapp.h>
#include <ecs/ecs.h>
#include <rl/systems/MoveAdj.h>

static const char * dirstrings_short[] = { "SW", "S", "SE", "W", "", "E", "NW", "N", "NE" };

namespace pgn
{
	namespace rl
	{
		void RegisterActions(ecs::cECS& ecs)
		{
			auto& sys = mainecs()->System<ecs::sys::cMoveAdj>();

			for (int i = -1; i <= 1; ++i)
			for (int j = -1; j <= 1; ++j)
			{
				int o = (i + 1) * 3 + j + 1;
				//cECS::cActionFunc func = std::bind(&evt::cPlayerMoveAdj::RunEvent, glm::ivec2(j, i));
				auto func = [&,i,j](){ 
					auto ed = mainecs()->TagusToEntities("Player");
					sys(ed->second, glm::ivec2(j, i));
				};

				ecs.RegisterAction(std::string("PlayerMove") + dirstrings_short[o], func);
			}
		}
	}
}