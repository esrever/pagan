#include "GameTurn.h"

#include <ecs/systems/InputKey.h>

#include <rl/components/components.h>
#include <rl/fov/FovLookup.h>
#include <rl/fov/fov_rsc.h>

#include "MoveAdj.h"
#include "UpdateAI.h"

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			static const float sMinTime = 1e-04f;
			//-------------------------------------------------------------------------
			cGameTurn::cGameTurn() :
				mActorQuery(cComponentQuery::ePolicy::Any),
				INIT_EVT_MEMBER(cGameTurn, PlayerAction),
				INIT_EVT_MEMBER(cGameTurn, AIAction),
				mCurrent(mActors.end())
			{
				mActorQuery.Require<cmp::cControllerAI>()
						   .Require<cmp::cControllerPlayer>();
				mActorQuery.SetOnEntityAdded([&](ecs::cEntityWithData ed)
				{
					float t = mActors.empty() ? 0.0f : mActors.back().first + sMinTime;
					mActors.push_back(std::make_pair(t, ed));
				});
				mActorQuery.SetOnEntityRemoved([&](ecs::cEntityWithData ed)
				{
					auto itf = std::find_if(mActors.begin(), mActors.end(), [&](const data_type& kv) {return kv.second->first == ed->first; });
					assert(itf != mActors.end());
					mActors.erase(itf); 
				});
			}

			//-------------------------------------------------------------------------
			void cGameTurn::SetCurrent(ecs::cEntityWithData ed)
			{
				auto itf = std::find_if(mActors.begin(), mActors.end(), [&](const data_type& kv) {return kv.second->first == ed->first; });
				assert(itf != mActors.end());
				mCurrent = itf;
			}

			//-------------------------------------------------------------------------
			void cGameTurn::OnPlayerAction(float tu)
			{
				if (Active())
				{
					Advance(tu);
					// Disable keyboard till it's player's turn again
					mainecs()->System<cInputKey>().SetActive(false);
				}
			}

			//-------------------------------------------------------------------------
			void cGameTurn::OnAIAction(float tu)
			{
				if (Active())
					Advance(tu);
			}

			//-------------------------------------------------------------------------
			void cGameTurn::Advance(float tu)
			{				
				// Calc next node
				auto next = std::next(mCurrent);
				if (next == mActors.end())
					next = mActors.begin();

				// Calc next time mCurrent will play
				float tNext = mCurrent->first + tu;
				// if it has issued a wait command
				if (tu == 0.0f)
				{
					// if there's nobody around
					if (next == mCurrent)
						// wait a second
						tNext = mCurrent->first + 1.0f;
					else // else play immediately after next
						tNext = next->first + sMinTime;
				}

				mCurrent->first = tNext;
				mActors.sort();
				if (next->first < mCurrent->first)
					mCurrent = next;
			}

			 
			//-------------------------------------------------------------------------
			bool cGameTurn::operator()()
			{
				if (!Active())
					return false;
				auto start = mCurrent->second->first;
				// Run current
				do
				{
					auto cmp_ai = mCurrent->second->second.Component<cmp::cControllerAI>();
					auto cmp_pc = mCurrent->second->second.Component<cmp::cControllerPlayer>();
					if (cmp_pc)
					{
						// Enable keyboard
						mainecs()->System<cInputKey>().SetActive(true);
						break;
					}
					else if (cmp_ai)
					{
						mainecs()->System<cUpdateAI>()(mCurrent->second);
					}
					else
						assert(false);
				} while (mCurrent->second->first != start); // if we do full circle, go out for a render
				return true;
			}
		}
	}
}