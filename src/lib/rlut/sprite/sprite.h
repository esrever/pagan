#pragma once

#include <core/serialize/serialize.h>
#include <core/sdl/pointers.h>

namespace pgn
{
	namespace rlut
	{
		typedef std::pair<SDL_Texture *, SDL_Rect> cSprite;
	}
	
	void SerializeOut(node_type& writer, const std::string& key, const rlut::cSprite & value);
	bool SerializeIn(const node_type& reader, rlut::cSprite & value);
}