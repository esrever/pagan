#include "font.h"

#include <stdexcept>

namespace pgn
{
	cSDLFont::cSDLFont(SDL_Renderer * renderer, const std::string& fontName, size_t fontSize)
	:mRenderer(renderer)
	{
		mFont = TTF_OpenFont(fontName.c_str(), fontSize);
		if (mFont == nullptr)
			throw std::runtime_error("Failed to load font: " + fontName + TTF_GetError());
	}

	cSDLFont::~cSDLFont()
	{
		TTF_CloseFont(mFont);
	}

	cSDLTexture_uptr cSDLFont::CreateText(const std::string& message, SDL_Rect * rect, const SDL_Color& c)
	{
		SDL_Surface *surf = TTF_RenderText_Blended(mFont, message.c_str(), c);
		if (rect)
		{
			rect->w = surf->w;
			rect->h = surf->h;
		}
		auto tex = SDLTexture_uptr(SDL_CreateTextureFromSurface(mRenderer, surf));
		//Clean up unneeded stuff
		SDL_FreeSurface(surf);
		return tex;
	}
}