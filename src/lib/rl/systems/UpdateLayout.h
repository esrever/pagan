#pragma once

#include <core/event/Event.h>
#include <ecs/events.h>
#include <ecs/system.h>
#include <ecs/ecs.h>

#include <rl/event/events.h>
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
					cUpdateLayout();
					void OnLevelCreated(ecs::cEntityWithData ed);
					bool operator()(ecs::cEntityWithData& ed, cmp::cLocation * zOldLoc);
				private:
					void AddToLayout(ecs::cEntityWithData& ed, ecs::cEntityWithData lvl);
					void RemoveFromLayout(ecs::cEntityWithData& ed, ecs::cEntityWithData lvl, const glm::ivec2& oldPos);
					void UpdateLayout(ecs::cEntityWithData& ed, ecs::cEntityWithData lvl, const glm::ivec2& oldPos);

					void GenerateStaticMoveCosts(ecs::cEntityWithData lvl);
					void GenerateMoveCosts(ecs::cEntityWithData lvl);
					void GenerateStaticObstacles(ecs::cEntityWithData lvl);
					void GenerateObstacles(ecs::cEntityWithData lvl);

					void UpdateStaticMoveCosts(ecs::cEntityWithData lvl, const glm::ivec2& pos);
					void UpdateMoveCosts(ecs::cEntityWithData lvl, const glm::ivec2& pos);
					void UpdateStaticObstacles(ecs::cEntityWithData lvl, const glm::ivec2& pos);
					void UpdateObstacles(ecs::cEntityWithData lvl, const glm::ivec2& pos);
				private:
					DECL_EVT_MEMBER(LevelCreated);
			};
		}
	}
}