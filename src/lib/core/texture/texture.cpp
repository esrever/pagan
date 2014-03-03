#include "texture.h"

namespace pgn
{
	cSubTexture cTexture::SubTexture(const SDL_Rect * rect) 
	{ 
		SDL_Rect rdef = { 0, 0, 0, 0 };
		return cSubTexture(this, rect ? *rect : rdef);
	}

	void cTexture::Load(texture_loadfunc_type func, const std::string& fname, const char * desc)
	{
		mTexture = func(fname);
		mName = desc ? desc : fname;
	}
}