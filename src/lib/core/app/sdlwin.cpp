#include "sdlwin.h"

#include <core/texture/texturelib.h>

namespace pgn
{
	cSDLWindow::cSDLWindow(const SDL_Rect& rect, const std::string& caption, Uint32 flags)
	: mRenderer(SDLRenderer_sptr())
	, mWindow(SDLWindow_uptr())
	{
		mRect = rect;
		if ((mRect.w == 0) || (mRect.h == 0))
		{
			SDL_DisplayMode dmode;
			SDL_GetCurrentDisplayMode(0, &dmode);
			mRect.w = dmode.w;
			mRect.h = dmode.h;
			mRect.x = mRect.y = 0;
		}
		mWindow = SDLWindow_uptr(SDL_CreateWindow(caption.c_str(), mRect.x, mRect.y, mRect.w, mRect.h, flags));
		if (mWindow == nullptr)
			throw std::runtime_error("Failed to create window");

		mRenderer = SDLRenderer_sptr(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED));
			//Make sure it created ok
		if (mRenderer == nullptr)
			throw std::runtime_error("Failed to create renderer");
		
		mTextureLib = new cTextureLib(mRenderer.get());

		// Create the default texture
		SDL_Color pixel = { 255, 255, 255, 255 };
		SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(&pixel, 1, 1, 32, 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		mDefaultTexture = SDL_CreateTextureFromSurface(mRenderer.get(), surf);
		mTextureLib->Store( std::make_shared<cTexture>(cTexture(mDefaultTexture, "default")));
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
		RenderEx(mDefaultTexture, col, NULL, rect);
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