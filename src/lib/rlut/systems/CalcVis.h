#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <rlut/events.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cCalcVis : public cBase
			{
				public:
					cCalcVis();
					void OnLocationChanged(const ecs::cEntityWithData& evt);
					bool operator()(ecs::cEntityWithData&);

				private:
					DECL_EVT_MEMBER(LocationChanged);
			};
		}
	}
}