#include "CreateLevel.h"

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
			cCreateLevel::cCreateLevel(){}

			//-------------------------------------------------------------------------
			bool cCreateLevel::operator()(ecs::cArchetypeWithData& arch)
			{
				auto ed = mainecs()->InstantiateArchetype(arch->second);
				auto level = ed->second.Component<ecs::cmp::cLevelData>();
				auto world = mainecs()->TagusToEntities("World");
				world->second->second.Component<ecs::cmp::cWorldData>()->mLevelMap[ed->first] = ed;
				evt::cLevelCreated::Sig().emit(ed);
				return true;
			}
		}
	}
}