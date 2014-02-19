#pragma once

#include <glm/glm.hpp>

inline bool operator < (const glm::ivec2& lhs, const glm::ivec2& rhs)
{
	return lhs.x < rhs.x
		? true
		: (lhs.x == rhs.x
			? lhs.y < rhs.y
			: false);
}

template<>
inline bool std::less<glm::ivec2>::operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) const
{
	return lhs.x < rhs.x
		? true
		: (lhs.x == rhs.x
		? lhs.y < rhs.y
		: false);
}

namespace glm
{
	template<class T>
	inline T max_element(const detail::tvec2<T>& v) { return std::max(v.x, v.y);  }
	template<class T>
	inline T max_element(const detail::tvec3<T>& v) { return std::max( std::max(v.x, v.y), v.z); }
	template<class T>
	inline T max_element(const detail::tvec4<T>& v) { return std::max(std::max(v.x, v.y), std::max(v.z, v.w)); }

	template<class T>
	inline T min_element(const detail::tvec2<T>& v) { return std::min(v.x, v.y); }
	template<class T>
	inline T min_element(const detail::tvec3<T>& v) { return std::min(std::min(v.x, v.y), v.z); }
	template<class T>
	inline T min_element(const detail::tvec4<T>& v) { return std::min(std::min(v.x, v.y), std::min(v.z, v.w)); }


	template<class T>
	inline T sum(const detail::tvec2<T>& v) { return v.x+v.y; }
	template<class T>
	inline T sum(const detail::tvec3<T>& v) { return v.x + v.y + v.z; }
	template<class T>
	inline T sum(const detail::tvec4<T>& v) { return v.x+v.y+v.z+v.w; }
}