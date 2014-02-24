#include "sdlwin.h"

namespace pgn
{
	cSDLWindow::cSDLWindow(const SDL_Rect& rect, const std::string& caption, Uint32 flags)
	:mDefaultTexture(SDLTexture_uptr())
	, mRenderer(SDLRenderer_sptr())
	, mWindow(SDLWindow_uptr())
	{
		mRect = rect;
		mWindow = SDLWindow_uptr(SDL_CreateWindow(caption.c_str(), rect.x, rect.y, rect.w, rect.h, flags));
		if (mWindow == nullptr)
			throw std::runtime_error("Failed to create window");

		mRenderer = SDLRenderer_sptr(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED));
			//Make sure it created ok
		if (mRenderer == nullptr)
			throw std::runtime_error("Failed to create renderer");

		// Create the default texture
		SDL_Color pixel = { 255, 255, 255, 255 };
		SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(&pixel, 1, 1, 32, 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		mDefaultTexture = SDLTexture_uptr(SDL_CreateTextureFromSurface(mRenderer.get(), surf));
		SDL_FreeSurface(surf);
	}

	cSDLWindow::~cSDLWindow()
	{
	}

	void cSDLWindow::Render(SDL_Texture * tex, const SDL_Rect * rect)
	{
		const SDL_Color c = { 255, 255, 255, 255 };
		RenderEx(tex,c , NULL, rect);
	}

	void cSDLWindow::Render(const SDL_Color& col, const SDL_Rect * rect)
	{
		RenderEx(mDefaultTexture.get(), col, NULL, rect);
	}

	void cSDLWindow::RenderEx(SDL_Texture * tex, const SDL_Color& col, const SDL_Rect * srcrect, const SDL_Rect * dstrect)
	{
		SDL_SetTextureColorMod(tex, col.r, col.g, col.b);
		SDL_SetTextureAlphaMod(tex, col.a);
		SDL_RenderCopy(mRenderer.get(), tex, srcrect, dstrect);
		SDL_SetTextureColorMod(tex, 255, 255, 255);
		SDL_SetTextureAlphaMod(tex, 255);
	}
}