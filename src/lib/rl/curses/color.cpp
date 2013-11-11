#include "color.h"

#include <curses.h>

namespace pgn
{
	namespace curses
	{
		std::map<uint32_t, uint16_t> cColor::mColorPairToIndex;
		std::map<uint64_t, uint16_t> cColor::mColorToIndex;

		uint16_t cColor::CreateColorIndex(const glm::vec3& zCol)
		{
			auto c = glm::clamp(glm::uvec3(glm::vec3(1000)*zCol), 0, 1000);
			uint64_t idx1 = uint64_t(c.x) + (uint64_t(c.y) << 16) + (uint64_t(c.z) << 32);
			auto it = mColorToIndex.find(idx1);
			if (it == mColorToIndex.end())
			{
				uint16_t val = uint16_t(mColorToIndex.size());
				mColorToIndex[idx1] = val;
				init_color(val, c.x, c.y, c.z);
				return val;
			}
			else
				return it->second;
		}

		uint16_t cColor::CreateColorPairIndex(const glm::vec3& zFront, const glm::vec3& zBack)
		{
			if (mColorPairToIndex.empty())
			{
				start_color();
				CreateColorIndex(glm::vec3(0,0,0));
				CreateColorIndex(glm::vec3(0, 0, 1));
				CreateColorIndex(glm::vec3(0, 1, 0));
				CreateColorIndex(glm::vec3(0, 1, 1));
				CreateColorIndex(glm::vec3(1, 0, 0));
				CreateColorIndex(glm::vec3(1, 0, 1));
				CreateColorIndex(glm::vec3(1, 1, 0));
				CreateColorIndex(glm::vec3(1, 1, 1));
			}

			auto cf = CreateColorIndex(zFront);
			auto cb = CreateColorIndex(zBack);

			auto pairIdx = glm::u16vec2(cf, cb);
			uint32_t pairIdx1 = pairIdx.x + (pairIdx.y << 16);
			auto it = mColorPairToIndex.find(pairIdx1);
			if (it == mColorPairToIndex.end())
			{
				uint16_t val = uint16_t(mColorPairToIndex.size() + 1);
				mColorPairToIndex[pairIdx1] = val;
				init_pair(val, pairIdx.x, pairIdx.y);
				return val;
			}
			else
				return it->second;
		}
	}
}