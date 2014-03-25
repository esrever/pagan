#pragma once

#include <string>
#include <SDL_ttf.h>

#include <core/sdl/util.h>

namespace pgn
{
	class cSDLFont
	{
	public:
		cSDLFont(SDL_Renderer * renderer, const std::string& fontName = "", size_t fontSize = 16);
		~cSDLFont();

		cSDLTexture_uptr CreateText(const std::string& message, SDL_Rect * rect = nullptr, const SDL_Color& c = { 255, 255, 255, 255 });
	private:
		TTF_Font * mFont;
		SDL_Renderer * mRenderer;
	};
}