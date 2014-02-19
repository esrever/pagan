#pragma once

#include <algorithm>

#include <core/util/glm_utils.h>
#include <glm/glm.hpp>

namespace pgn
{
	template<class T>
	inline typename T::value_type norm_1(const T& v) { return glm::sum(glm::abs(v)); }

	template<class T>
	inline float norm_2(const T& v) { return sqrtf(float(glm::sum(v*v))); }

	template<class T>
	inline typename T::value_type norm_inf(const T& v) { return glm::max_element(glm::abs(v)); }
}