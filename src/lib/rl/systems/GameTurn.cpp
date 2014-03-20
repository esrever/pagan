#include "GameTurn.h"

#include <rl/components/components.h>
#include <rl/fov/FovLookup.h>
#include <rl/fov/fov_rsc.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//-------------------------------------------------------------------------
			cGameTurn::cGameTurn() :
				INIT_EVT_MEMBER(cGameTurn, EntityCreated),
				INIT_EVT_MEMBER(cGameTurn, EntityDestroy),
				INIT_EVT_MEMBER(cGameTurn, ComponentAdded)
				{}

			//-------------------------------------------------------------------------
			void cGameTurn::OnEntityCreated(ecs::cEntityWithData ed)
			{
				if ( ed->second.Component<cmp::cControllerAI>() || 
					 ed->second.Component<cmp::cControllerPlayer>())
				mActors.push_back(ed);
			}

			//-------------------------------------------------------------------------
			void cGameTurn::OnEntityDestroy(ecs::cEntityWithData ed)
			{
				if (ed->second.Component<cmp::cControllerAI>() ||
					ed->second.Component<cmp::cControllerPlayer>())
				{
					auto itf = std::find(mActors.begin(), mActors.end(), ed);
					assert(itf != mActors.end());
					mActors.erase(itf);
				}
			}

			//-------------------------------------------------------------------------
			void cGameTurn::OnComponentAdded(ecs::cEntityWithData ed, int id)
			{
				if ( (id == cComponent<cmp::cControllerAI>::StaticTypeIndex()) ||
					(id == cComponent<cmp::cControllerPlayer>::StaticTypeIndex()))
					mActors.push_back(ed);
			}

			//-------------------------------------------------------------------------
			bool cGameTurn::operator()()
			{
				return true;
			}
		}
	}
}