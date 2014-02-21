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
		struct cFov1
		{
			cFov1() :mLoS(0){}

			void Calc(const glm::ivec2& p, const cArray2D<bool>& vismap, std::vector<glm::ivec2>& lospts, cArray2D<float>& vis);
			void Init(size_t los);
			bool HasInit() const { return mLoS != 0; }
			
			size_t mLoS;
		};
	}
}
