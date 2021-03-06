#pragma once

#include <vector>
#include <SDL.h>
#include <SDL_pixels.h>

namespace pgn
{
	SDL_Surface * ResizeSurface(SDL_Surface * surface, size_t tgtW, size_t tgtH);
}