#pragma once

#include "Array2D.h"

namespace pgn
{
	template<class T>
	struct Array2D_compare_less {
		Array2D_compare_less(const cArray2D<T>& ref) :mRef(ref){}
		bool operator() (const glm::ivec2& lhs, const glm::ivec2& rhs) const{
			return mRef(lhs) < mRef(rhs);
		}
		const cArray2D<T>& mRef;
	};
}