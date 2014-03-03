#include "texture.h"

namespace pgn
{
	cSubTexture cTexture::SubTexture(const SDL_Rect * rect) const 
	{ 
		return cSubTexture(*this, rect ? *rect : SDL_Rect()); 
	}
}