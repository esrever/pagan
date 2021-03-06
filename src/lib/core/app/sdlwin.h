#pragma once

#include <string>

#include <SDL.h>
#include <core/sdl/util.h>

namespace pgn
{
	class cTextureLib;

	class cSDLWindow
	{
	public:
		cSDLWindow(const SDL_Rect& rect = { 0, 0, 0/*1366*/, 0/*728*/ }, const std::string& caption = "Default", Uint32 flags = SDL_WINDOW_BORDERLESS);
		virtual ~cSDLWindow();

		SDL_Renderer * Renderer() { return mRenderer.get(); }
		void Render(SDL_Texture * tex, const SDL_Rect * rect = nullptr);
		void Render(const SDL_Color& col, const SDL_Rect * rect = nullptr);

		void RenderEx(SDL_Texture * tex, const SDL_Color& col, const SDL_Rect * srcrect, const SDL_Rect * dstrect);

		const SDL_Rect& Rect() const { return mRect; }
		cTextureLib * TextureLib() { return mTextureLib; }
	private:
		SDL_Rect mRect;
		cSDLWindow_uptr mWindow;
		cSDLRenderer_sptr mRenderer;
		SDL_Texture * mDefaultTexture;
		cTextureLib * mTextureLib;
	};
}