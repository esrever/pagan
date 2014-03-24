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
			class cRenderGameMap : public cBase
			{
			public:
				bool operator()();
				void SetWindow();
				void SetArea(const glm::uvec2& start, const glm::uvec2& dims, size_t tileDim);
			private:
				void RenderTile(ecs::cEntityWithData ed, const glm::ivec2& pos, float vis);
				glm::ivec2 ScreenPos(const glm::ivec2& tilepos, const glm::ivec2& cpos);
			private:

				glm::uvec2 mGridStart;
				glm::uvec2 mGridSize;
				size_t	   mTileSize;
			};
		}
	}
}