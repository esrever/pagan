#pragma once

#include <utility>
#include <string>

struct SDL_Texture;

namespace pgn
{
	typedef std::pair<SDL_Texture *, std::string> cTexture;
}