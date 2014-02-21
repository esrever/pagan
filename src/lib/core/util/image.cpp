#include "image.h"

#include <iostream>

namespace pgn
{
	SDL_Surface * ResizeSurface(SDL_Surface * surface, size_t tgtW, size_t tgtH)
	{
		SDL_Surface * newSurface = SDL_CreateRGBSurface(0, tgtW, tgtH, 
														surface->format->BitsPerPixel, 
														surface->format->Rmask, 
														surface->format->Gmask, 
														surface->format->Bmask, 
														surface->format->Amask);
		auto err = SDL_BlitScaled(surface, NULL, newSurface, NULL);
		std::cout<< SDL_GetError()<<std::endl;

		unsigned char * pixels = (unsigned char *)(newSurface->pixels);
		for (size_t i = 0; i < (tgtW*tgtH); ++i)
		{
			pixels[i * 4 + 3] = 255;
			pixels[i * 4 + 2] = 128;
		}

		return newSurface;
	}
}