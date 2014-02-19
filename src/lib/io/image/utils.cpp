#include "utils.h"

#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_image.h>

namespace pgn 
{
	namespace io
	{
		void SavePaletteImage(const std::string& fname,
			const cArray2D<char>& cidxMap,
			const std::vector<SDL_Color>& colorMap)
		{
			void * pixels = (void *)&cidxMap.View().Storage().Raw().front();
			auto * surface = SDL_CreateRGBSurfaceFrom(pixels, cidxMap.Width(), cidxMap.Height(), 8, cidxMap.Width(), 0, 0, 0, 0);
			auto * palette = surface->format->palette;

			// TODO: more efficient use of colormap: create a palette with only what's used. use unique.
			SDL_SetPaletteColors(palette, &colorMap.front(), 0, colorMap.size());

			IMG_SavePNG(surface, fname.c_str());

			SDL_FreeSurface(surface);
		}
	}
}