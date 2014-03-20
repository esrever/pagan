#include "GameTurn.h"

#include <rl/components/components.h>
#include <rl/fov/FovLookup.h>
#include <rl/fov/fov_rsc.h>

#include "MoveAdj.h"

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
				INIT_EVT_MEMBER(cGameTurn, ComponentAdded),
				INIT_EVT_MEMBER(cGameTurn, PlayerAction)
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
			void cGameTurn::SetCurrent(ecs::cEntityWithData ed)
			{
				auto itf = std::find(mActors.begin(), mActors.end(), ed);
				assert(itf != mActors.end());
				mCurrent = itf;
			}

			//-------------------------------------------------------------------------
			void cGameTurn::OnPlayerAction()
			{
				Advance();
				// TODO: just disable keyboard
			}

			//-------------------------------------------------------------------------
			void cGameTurn::Advance()
			{
				// Ok, changing now
				auto next = std::next(mCurrent);
				if (next == mActors.end())
					mCurrent = mActors.begin();
				else
					mCurrent = next;
			}


			//-------------------------------------------------------------------------
			bool cGameTurn::operator()()
			{
				// Run current
				do
				{
					auto cmp_ai = (*mCurrent)->second.Component<cmp::cControllerAI>();
					auto cmp_pc = (*mCurrent)->second.Component<cmp::cControllerPlayer>();
					if (cmp_pc)
					{
						// TODO: just enable keyboard
						break;
					}
					else if (cmp_ai)
					{
						// TODO: simulate normally, all of them till we reach the player
						mainecs()->System<cMoveAdj>()(*mCurrent, glm::ivec2((rand() % 3) - 1, (rand() % 3) - 1));
						Advance();
					}
					else
						assert(false);
				} while (true); // TODO: limit
				return true;
			}
		}
	}
}