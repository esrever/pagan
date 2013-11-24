#pragma once

#include <glm/glm.hpp>
#include <ecs/Entity.h>

namespace pgn
{
	namespace cmp
	{
	}

	namespace evt
	{
		void ActionMove(cEntity e, const glm::ivec2& loc);
	}
}