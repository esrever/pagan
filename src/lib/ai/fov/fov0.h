#pragma once

#include <memory>
#include <set>
#include <map>
#include <vector>

#include <core/container/Array2D.h>

namespace pgn
{
	namespace ai
	{
		struct cFov0
		{
			cFov0() :mLoS(0){}

			void Calc(const glm::ivec2& p, const cArray2D<bool>& vismap, std::vector<glm::ivec2>& lospts, cArray2D<float>& vis);
			void Init(size_t los);
			bool HasInit() const { return mLoS != 0; }

			std::map<size_t, std::vector<glm::ivec2>> mTierMap;
			cArray2D<float> mVisMap;
			cArray2D<bool> mBlockMap;
			
			size_t mLoS;
		};
	}
}
