#include "RenderStatus.h"

#include <core/texture/texturelib.h>
#include <core/texture/subtexturelib.h>
#include <core/sdl/util.h>

#include <rl/event/events.h>
#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cRenderStatus::operator()()
			{
				return false;
			}
		}
	}
}