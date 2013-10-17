#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace pgn
{
	//! World position coordinates
	struct cLocation
	{
		unsigned   mLevelId;
		glm::ivec2 mCoords;
	};

	template<>
	inline std::string to_string<cLocation>(const cLocation& s)
	{
		return boost::str( boost::format("(%d,%d)-%d")%s.mCoords.x%s.mCoords.y%s.mLevelId);
	}
}