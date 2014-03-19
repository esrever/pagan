#include "CreateLevel.h"

#include <rlut/components/components.h>
#include <rlut/tile/layout.h>
#include <rlut/fov/FovLookup.h>
#include <rlut/fov/fov_rsc.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//-------------------------------------------------------------------------
			cCreateLevel::cCreateLevel() :INIT_EVT_MEMBER(cCreateLevel, LevelCreated){}

			//-------------------------------------------------------------------------
			void cCreateLevel::OnLevelCreated(const ecs::cEntityWithData& ed)
			{
				if (!Active()) return;
				//(*this)(const_cast<ecs::cEntityWithData&>(evt));
				// TODO: flesh out. Currently store the id -> level association
				auto level = ed->second.Component<ecs::cmp::cLevelData>();
				auto world = mainecs()->TagusToEntities("World");
				world->second->second.Component<ecs::cmp::cWorldData>()->mLevelMap[ed->first] = ed;
			}

			//-------------------------------------------------------------------------
			bool cCreateLevel::operator()(ecs::cEntityWithData& ed)
			{
				return false;
			}
		}
	}
}