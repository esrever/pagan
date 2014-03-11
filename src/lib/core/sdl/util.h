#pragma once

#include <SDL_rect.h>
#include <core/serialize/serialize.h>

namespace std
{
	inline const string to_string(const SDL_Rect& v)
	{
		std::ostringstream oss;
		oss << "{" << v.x << "," << v.y << "," << v.w << "," << v.h << "}";
		return oss.str();
	}

	inline const SDL_Rect from_string(const std::string& v)
	{
		SDL_Rect rect;
		sscanf(v.c_str(), "{%d,%d,%d,%d}", rect.x, rect.y, rect.w, rect.h);
		return rect;
	}
}
