#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <rlut/event/events.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cCreateLevel : public cBase
			{
				public:
					cCreateLevel();
					void OnLevelCreated(const ecs::cEntityWithData& evt);
					bool operator()(ecs::cEntityWithData&);

				private:
					DECL_EVT_MEMBER(LevelCreated);
			};
		}
	}
}