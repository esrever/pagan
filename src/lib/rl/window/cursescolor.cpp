#include "cursescolor.h"

#include <curses.h>

namespace pgn
{
	std::map<uint32_t, uint16_t> cCursesColor::mColorPairToIndex;
	std::map<uint64_t, uint16_t> cCursesColor::mColorToIndex;

	uint16_t cCursesColor::CreateColorIndex(const glm::vec3& zCol)
	{
		auto c = glm::clamp( glm::uvec3(glm::vec3(1000)*zCol),0,1000);
		uint64_t idx1 = uint64_t(c.x) + (uint64_t(c.y)<<16) + (uint64_t(c.z)<<32);
		auto it = mColorToIndex.find(idx1);
		if( it == mColorToIndex.end())
		{
			uint16_t val = uint16_t(mColorToIndex.size());
			mColorToIndex[idx1] = val;
			init_color(val, c.z, c.y, c.x);
			return val;
		}
		else
			return it->second;
	}

	uint16_t cCursesColor::CreateColorPairIndex(const glm::vec3& zFront, const glm::vec3& zBack)
	{
		if(mColorPairToIndex.empty())
			start_color();

		auto cf = CreateColorIndex(zFront);
		auto cb = CreateColorIndex(zBack);

		auto pairIdx = glm::u16vec2(cf,cb);
		uint32_t pairIdx1 = pairIdx.x + (pairIdx.y<<16);
		auto it = mColorPairToIndex.find(pairIdx1);
		if( it == mColorPairToIndex.end())
		{
			uint16_t val = uint16_t(mColorPairToIndex.size()+1);
			mColorPairToIndex[pairIdx1] = val;
			init_pair(val, pairIdx.x,pairIdx.y);
			return val;
		}
		else
			return it->second;
	}
}