#pragma once

#include <memory>

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>
#include <core/app/sdlwin.h>
#include <rl/tile/tilestore.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cRenderGameMap : public cBase
			{
			public:
				bool operator()();
				void SetWindow(cSDLWindow * win) { mWindow = win; }
				void SetArea(const glm::uvec2& start, const glm::uvec2& dims, size_t tileDim);
			private:
				void RenderDense1(const rl::cTileStoreDense1&, const glm::ivec2& start, const glm::ivec2 size, std::function<int(int x, int y)> visfunc);
				void RenderSparse1(const rl::cTileStoreSparse1&, const glm::ivec2& start, const glm::ivec2 size, std::function<int(int x, int y)> visfunc);

				void RenderTile(ecs::cEntityWithDataConst ed, const glm::ivec2& pos, const glm::ivec2& sspos, int vis);
				glm::ivec2 ScreenPos(const glm::ivec2& tilepos, const glm::ivec2& cpos);
			private:

				cSDLWindow * mWindow;
				glm::uvec2 mGridStart;
				glm::uvec2 mGridSize;
				size_t	   mTileSize;
			};
		}
	}
}