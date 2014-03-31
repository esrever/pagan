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
					typedef std::pair<float, ecs::cEntityWithData> data_type;
					typedef std::list<data_type> container_type;
						
					cGameTurn();
					void OnPlayerAction( float tu);
					void OnAIAction(float tu);

					bool operator()();
					void SetCurrent(ecs::cEntityWithData ed);
				private:
					//! Set up when the current entity will play again and update current
					void Advance(float tu);
				private:
					DECL_EVT_MEMBER(PlayerAction);
					DECL_EVT_MEMBER(AIAction);

					cComponentQuery mActorQuery;
					container_type mActors;
					container_type::iterator mCurrent;
			};
		}
	}
}