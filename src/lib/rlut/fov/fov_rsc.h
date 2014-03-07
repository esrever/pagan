#pragma once

#include <memory>
#include <set>
#include <map>
#include <vector>

#include <core/container/Array2D.h>

namespace pgn
{
	namespace rlut
	{
		struct cFovRsc
		{
			cFovRsc() :mLoS(0){}

			/**
			 * Calculates the points in LoS and their visibility [0: invisible, 1: fully visible]
			 * @param p		 Eye coordinates
			 * @param vismap Visibility map
			 * @param lospts Points in LoS
			 * @param vis	 Visibility value
			 * TODO: vis should be a vector, like lospts?
			 * TODO: vis should be optional?
			 * TODO: lospts should be optional?
			 */
			void Calc(const glm::ivec2& p, const cArray2D<bool>& vismap, std::vector<glm::ivec2>& lospts, cArray2D<float>& vis);
			void Init(size_t los);
			bool HasInit() const { return mLoS != 0; }
			
			size_t mLoS;
		};
	}
}
