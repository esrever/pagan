#pragma once

#include <glm/glm.hpp>

namespace pgn
{
	//! World position coordinates
	struct cPosition
	{
		uint	   mLevelId;
		glm::ivec2 mCoords;
	};
}