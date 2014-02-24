#pragma once

#include <string>

#include <SDL.h>
#include <core/sdl/pointers.h>

namespace pgn
{
	class cSDLWindow
	{
	public:
		cSDLWindow(const SDL_Rect& rect = { 10, 10, 1340, 700 }, const std::string& caption = "Default",Uint32 flags = 0);
		virtual ~cSDLWindow();

		SDL_Renderer * Renderer() { return mRenderer.get(); }
		void Render(SDL_Texture * tex, const SDL_Rect * rect = nullptr);
		void Render(const SDL_Color& col, const SDL_Rect * rect = nullptr);

		void RenderEx(SDL_Texture * tex, const SDL_Color& col, const SDL_Rect * srcrect, const SDL_Rect * dstrect);

		const SDL_Rect& Rect() const { return mRect; }
	private:
		SDL_Rect mRect;
		cSDLWindow_uptr mWindow;
		cSDLRenderer_sptr mRenderer;
		cSDLTexture_uptr mDefaultTexture;
	};
}