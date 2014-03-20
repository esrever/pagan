#pragma once

#include <list>

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/events.h>
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
					void OnEntityCreated(ecs::cEntityWithData ed);
					void OnEntityDestroy(ecs::cEntityWithData ed);
					void OnComponentAdded(ecs::cEntityWithData ed, unsigned short);
					void OnPlayerAction();

					bool operator()();
					void SetCurrent(ecs::cEntityWithData ed);
				private:
					void Advance();
				private:
					DECL_EVT_MEMBER(EntityCreated);
					DECL_EVT_MEMBER(EntityDestroy);
					DECL_EVT_MEMBER(ComponentAdded);
					DECL_EVT_MEMBER(PlayerAction);

					std::list<ecs::cEntityWithData> mActors;
					std::list<ecs::cEntityWithData>::iterator mCurrent;
			};
		}
	}
}