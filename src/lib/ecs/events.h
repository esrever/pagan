#pragma once

#include <ecs/ecs.h>

namespace pgn
{
	namespace evt
	{
		typedef std::tuple< cECS::cEntityWithData > cEntityCreated;
		typedef std::tuple< cECS::cEntityWithData > cEntityDestroy;
	}
}