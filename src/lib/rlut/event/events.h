#pragma once

#include <core/event/Event.h>
#include <ecs/ecs.h>
#include <rlut/components/components.h>

#define DECL_PEVT0(T) DECL_EVT1( Player##T, ecs::cEntityWithData );
#define DECL_PEVT1(T, A0) DECL_EVT2( Player##T , ecs::cEntityWithData, A0 );
#define DECL_PEVT2(T, A0, A1) DECL_EVT3( Player##T , ecs::cEntityWithData, A0 , A1 );

namespace pgn
{
	//! Event-specific stuff

	namespace evt
	{
		DECL_EVT1(LocationChanged, ecs::cEntityWithData);
		DECL_EVT1(LevelCreated, ecs::cEntityWithData);
		DECL_EVT1(LevelDestroy, ecs::cEntityWithData);
		/*
		DECL_EVT1(Appear, ecs::cEntityWithData)
		DECL_EVT1(Disappear, ecs::cEntityWithData)
		PlayerMoveAdj,
		PlayerAppear,
		CalculateVisibility,
		CreateLevel*/
	}
}