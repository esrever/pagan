#include "utils.h"

#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_image.h>

#include <core/util/image.h>

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

		void SaveImage(const std::string& fname, const cArray2D<SDL_Color>& cMap)
		{
			void * pixels = (void *)&cMap.View().Storage().Raw().front();
			auto * surface = SDL_CreateRGBSurfaceFrom(pixels, cMap.Width(), cMap.Height(), 32, cMap.Width() * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 );

			IMG_SavePNG(surface, fname.c_str());
			SDL_FreeSurface(surface);
		}

		void LoadImage(const std::string& fname, cArray2D<SDL_Color>& cmap)
		{
			auto surface = IMG_Load(fname.c_str());
			unsigned char * pixels = (unsigned char *)(surface->pixels);

			// write data to color array
			cmap.Resize(surface->w, surface->h);
			cmap.View().VisitWext([&](size_t x, size_t y, SDL_Color& c){
				auto o = (surface->h - 1 - y)*surface->pitch + 4 * x;
				c.r = pixels[o];
				c.g = pixels[o + 1];
				c.b = pixels[o + 2];
				c.a = pixels[o + 3];
			});

			SDL_FreeSurface(surface);
		}
	}
}