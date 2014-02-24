#pragma once

#include <memory>
#include <functional>

#include <SDL.h>

namespace pgn
{
#define DECL_SDLPTRF( P, F)\
	typedef std::unique_ptr< SDL_##P, void(*)(SDL_##P *) > cSDL##P##_uptr; \
	typedef std::shared_ptr< SDL_##P > cSDL##P##_sptr; \
	inline cSDL##P##_uptr SDL##P##_uptr(SDL_##P * tex = nullptr) { return cSDL##P##_uptr(tex, SDL_##F ); }\
	inline cSDL##P##_sptr SDL##P##_sptr(SDL_##P * tex = nullptr) { return cSDL##P##_sptr(tex, SDL_##F ); }

	DECL_SDLPTRF(Texture, DestroyTexture);
	DECL_SDLPTRF(Renderer, DestroyRenderer);
	DECL_SDLPTRF(Window, DestroyWindow);

#undef DECL_SDLPTRF
}