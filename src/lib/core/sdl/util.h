#pragma once

#include <SDL_rect.h>
#include <core/container/box2.h>

namespace pgn
{
	inline const cBox2i& as_rect(const SDL_Rect& r) { return *reinterpret_cast<const cBox2i*>(&r); }
	inline const SDL_Rect& as_sdlrect(const cBox2i& b) { return *reinterpret_cast<const SDL_Rect*>(&b); }
}
