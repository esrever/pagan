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

					bool operator()();
				private:
					DECL_EVT_MEMBER(EntityCreated);
					DECL_EVT_MEMBER(EntityDestroy);

					std::list<ecs::cEntityWithData> mActors;
			};
		}
	}
}