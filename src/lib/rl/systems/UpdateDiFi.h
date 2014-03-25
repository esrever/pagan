#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <rl/event/events.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cUpdateDiFi : public cBase
			{
				public:
					cUpdateDiFi();
					void OnLocationChanged(const ecs::cEntityWithData& evt);
					bool operator()(ecs::cEntityWithData&);

				private:
					DECL_EVT_MEMBER(LocationChanged);
			};
		}
	}
}