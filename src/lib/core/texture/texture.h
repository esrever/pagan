#pragma once

#include <utility>
#include <string>

struct SDL_Texture;
struct SDL_Rect;

namespace pgn
{
	typedef std::pair<SDL_Texture *, std::string> cTexture;
	typedef std::pair<cTexture, SDL_Rect *> cClippedTexture;
	typedef cClippedTexture cSprite;
}