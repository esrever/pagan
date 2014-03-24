#include "GameTurn.h"

#include <ecs/systems/InputKey.h>

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
				mActorQuery(cComponentQuery::ePolicy::Any),
				INIT_EVT_MEMBER(cGameTurn, PlayerAction)
			{
				mActorQuery.Require<cmp::cControllerAI>()
						   .Require<cmp::cControllerPlayer>();
				mActorQuery.SetOnEntityAdded([&](ecs::cEntityWithData ed){mActors.push_back(ed); });
				mActorQuery.SetOnEntityRemoved([&](ecs::cEntityWithData ed)
				{
					auto itf = std::find(mActors.begin(), mActors.end(), ed);
					assert(itf != mActors.end());
					mActors.erase(itf); 
				});
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
				// Disable keyboard till it's player's turn again
				mainecs()->System<cInputKey>().SetActive(false);
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
						// Enable keyboard
						mainecs()->System<cInputKey>().SetActive(true);
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