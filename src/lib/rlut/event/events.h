#pragma once

#include <core/event/Event.h>
#include <ecs/ecs.h>

namespace pgn
{

	//! Event-specific stuff
	namespace evt
	{
		enum class eRlEventId : size_t {
			MoveAdj = 256,
			Appear,
			Disappear
		};

		typedef cAction<size_t(eRlEventId::MoveAdj), cECS::cEntityWithData /*who*/, const glm::ivec2& /*dir*/> cMoveAdj;
		typedef cAction<size_t(eRlEventId::Appear), cECS::cEntityWithData/*who*/, const glm::ivec2& /*pos*/> cAppear;
		typedef cAction<size_t(eRlEventId::Disappear), cECS::cEntityWithData/*who*/, const glm::ivec2&/*last_pos*/> cDisappear;
	}

	namespace rl
	{
		void RegisterActions(cECS& ecs);
	}
}