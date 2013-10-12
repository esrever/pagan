#pragma once

#include <glm/glm.hpp>

namespace pgn
{
	//! World position coordinates
	struct cLocation
	{
		unsigned   mLevelId;
		glm::ivec2 mCoords;
	};
}