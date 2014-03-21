#include "UnloadLevel.h"

#include <rl/components/components.h>
#include <rl/tile/layout.h>
#include <rl/fov/FovLookup.h>
#include <rl/fov/fov_rsc.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//-------------------------------------------------------------------------
			cUnloadLevel::cUnloadLevel(){}

			//-------------------------------------------------------------------------
			bool cUnloadLevel::operator()(ecs::cEntityWithData& ed)
			{
				return false;
			}
		}
	}
}