#pragma once

#include <vector>
#include <SDL.h>
#include <SDL_pixels.h>

namespace pgn
{
	void GenGrayscaleGradient(std::vector<SDL_Color>& grad, Uint8 start = 0, Uint8 end = 255, size_t num = 256);
}