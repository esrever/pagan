#pragma once

#include <string>

#include <core/container/Array2D.h>

struct SDL_Color;

namespace pgn
{
	namespace io
	{
		void SavePaletteImage(const std::string& fname,
			const cArray2D<char>& cidxMap,
			const std::vector<SDL_Color>& colorMap);
	}
}