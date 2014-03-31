#pragma once

#include <list>

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/events.h>
#include <ecs/componentquery.h>
#include <rl/event/events.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cGameTurn : public cBase
			{
				public:
					cGameTurn();
					void OnPlayerAction( float tu);
					void OnAIAction(float tu);

					bool operator()();
					void SetCurrent(ecs::cEntityWithData ed);
				private:
					void Advance(float tu);
				private:
					DECL_EVT_MEMBER(PlayerAction);
					DECL_EVT_MEMBER(AIAction);

					cComponentQuery mActorQuery;
					std::list<ecs::cEntityWithData> mActors;
					std::list<ecs::cEntityWithData>::iterator mCurrent;
			};
		}
	}
}