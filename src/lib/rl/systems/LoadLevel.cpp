#include "LoadLevel.h"

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
			cLoadLevel::cLoadLevel(){}

			//-------------------------------------------------------------------------
			bool cLoadLevel::operator()(ecs::cEntityWithData& ed)
			{
				return false;
			}
		}
	}
}