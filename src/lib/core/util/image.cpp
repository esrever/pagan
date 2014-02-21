#include "image.h"

namespace pgn
{
	void ResizeSurface(SDL_Surface *& surface, size_t tgtW, size_t tgtH)
	{
		SDL_Surface * newSurface = SDL_CreateRGBSurface(0, tgtW, tgtH, 
														surface->format->BitsPerPixel, 
														surface->format->Rmask, 
														surface->format->Gmask, 
														surface->format->Bmask, 
														surface->format->Amask);
		SDL_SoftStretch(surface, NULL, newSurface, NULL);
		SDL_FreeSurface(surface);
		surface = newSurface;
	}
}