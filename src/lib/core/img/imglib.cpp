#include "imglib.h"

#include <SDL_image.h>

namespace pgn
{
	cImgLib::~cImgLib()
	{
		for (auto kv : mTextures)
			SDL_DestroyTexture(kv.second);
	}

	void cImgLib::Store(SDL_Texture * tex, const char * desc)
	{
		mTextures[desc] = tex;
	}

	SDL_Texture * cImgLib::Load(const char * fname, const char * desc)
	{
		SDL_Texture * tex = IMG_LoadTexture(mRenderer, fname);
		if (tex)
		{
			mTextures[desc ? desc : fname] = tex;
		}
		return tex;
	}

	void cImgLib::Unload(const char * desc)
	{
		auto it = mTextures.find(desc);
		if (it != mTextures.end())
		{
			auto tex = it->second;
			mTextures.erase(it);
			SDL_DestroyTexture(tex);
		}
	}
}