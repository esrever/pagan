#pragma once

#include <algorithm>
#include <vector>
#include <SDL.h>
#include <SDL_pixels.h>

#include <core/util/color.h>
#include <core/serialize/serialize.h>

namespace pgn
{
	void GenGrayscaleGradient(std::vector<SDL_Color>& grad, Uint8 start = 0, Uint8 end = 255, size_t num = 256);

	DECL_SERIALIZE_INTERFACE(SDL_Color);
}

namespace std
{
	inline bool operator < (const SDL_Color& lhs, const SDL_Color& rhs)
	//template<> inline bool std::less<SDL_Color>::operator()(const SDL_Color& lhs, const SDL_Color& rhs) const
	{
		return (*(unsigned *)(&lhs)) < (*(unsigned *)(&rhs));
	}
}