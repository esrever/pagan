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
					auto it = std::max_element(mActors.begin(), mActors.end(), [&](const data_type& lhs, const data_type& rhs) {return lhs.first < rhs.first; });
					float t = it == mActors.end() ? 0.0f : it->first + 1e-07f;
					auto itn = std::next(it);
					if (itn == mActors.end())
						itn = mActors.begin();
					mActors.insert( itn, std::make_pair(t,ed)); 
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
				Advance(tu);
				// Disable keyboard till it's player's turn again
				mainecs()->System<cInputKey>().SetActive(false);
			}

			//-------------------------------------------------------------------------
			void cGameTurn::OnAIAction(float tu)
			{
				Advance(tu);
			}

			//-------------------------------------------------------------------------
			void cGameTurn::Advance(float tu)
			{
				float thresh = 1e-07f;
				
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
						tNext = next->first + thresh;
				}

				mCurrent->first = tNext;

				// TODO: now, move element pointed by mCurrent and recalculate mCurrent.

				// Find the first node that will play later than mCurrent
				while (next->first <= tNext)
				{
					next = std::next(next);
					if (next == mActors.end())
						next = mActors.begin();
					if (next == mCurrent)
						break;
				};

				// move current to proper position, but store it first
				auto old_cur = mCurrent;
				mActors.splice(next, mActors, mCurrent);

				// calculate new current
				mCurrent = std::next(mCurrent);
				if (mCurrent == mActors.end())
					mCurrent = mActors.begin();
			}


			//-------------------------------------------------------------------------
			bool cGameTurn::operator()()
			{
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