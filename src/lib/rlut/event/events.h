#pragma once

#include <core/event/Event.h>
#include <ecs/ecs.h>
#include <rlut/components/components.h>

namespace pgn
{

	//! Event-specific stuff
	namespace evt
	{
		enum class eRlEventId : size_t {
			MoveAdj = 256,
			Appear,
			Disappear,
			PlayerMoveAdj,
			PlayerAppear,
			CalculateVisibility
		};
		
		typedef cAction<size_t(eRlEventId::MoveAdj), cECS::cEntityWithData /*who*/, const glm::ivec2& /*dir*/> cMoveAdj;
		typedef cAction<size_t(eRlEventId::Appear), cECS::cEntityWithData/*who*/, const rl::cmp::cLocation& /*pos*/> cAppear;
		typedef cAction<size_t(eRlEventId::Disappear), cECS::cEntityWithData/*who*/, const rl::cmp::cLocation&/*last_pos*/> cDisappear;

		typedef cAction<size_t(eRlEventId::PlayerMoveAdj), const glm::ivec2& /*dir*/> cPlayerMoveAdj;
		typedef cAction<size_t(eRlEventId::PlayerAppear), const rl::cmp::cLocation& /*pos*/> cPlayerAppear;

		typedef cAction<size_t(eRlEventId::CalculateVisibility), cECS::cEntityWithData/*who*/> cCalculateVisibility;
	}

	namespace rl
	{
		void RegisterActions(cECS& ecs);
	}
}