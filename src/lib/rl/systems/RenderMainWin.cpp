#include "RenderMainWin.h"

#include <core/texture/texturelib.h>
#include <core/texture/subtexturelib.h>
#include <core/sdl/util.h>

#include <rl/event/events.h>
#include <rl/components/components.h>

#include <rl/systems/RenderGameMap.h>
#include <rl/systems/RenderStatus.h>
#include <rl/systems/RenderLog.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cRenderMainWin::operator()()
			{
				auto& ecs = mainecs();
				ecs->System<cRenderGameMap>()();
				ecs->System<cRenderStatus>()();
				ecs->System<cRenderLog>()();
				return false;
			}

			void cRenderMainWin::SetWindow(cSDLWindow * win)
			{
				mWindow = win;
				auto& ecs = mainecs();
				ecs->System<cRenderGameMap>().SetWindow(win);
				ecs->System<cRenderStatus>().SetWindow(win);
				ecs->System<cRenderLog>().SetWindow(win);
			}

			void cRenderMainWin::SetLayout(const glm::ivec2& grid, size_t tileDim)
			{
				auto& ecs = mainecs();

				assert((grid.x * int(tileDim)-10) < mWindow->Rect().w);
				assert((grid.y * int(tileDim) - 10) < mWindow->Rect().h);
				
				ecs->System<cRenderGameMap>().SetArea(cBox2i(glm::ivec2(0, 0), grid), tileDim);

				const auto windims = glm::ivec2(mWindow->Rect().w, mWindow->Rect().h);
				
				const auto sstart = glm::ivec2(grid.x*tileDim + 5, 5);
				const auto ssize = glm::ivec2(windims.x - sstart.x - 5);
				ecs->System<cRenderStatus>().SetArea(cBox2i(sstart, ssize));

				const auto logstart = glm::ivec2(5, tileDim*grid.y + 5);
				const auto logsize = glm::ivec2(grid.x*tileDim, windims.y - logstart.y - 5);
				ecs->System<cRenderLog>().SetArea(cBox2i(logstart, logsize));
			}
		}
	}
}