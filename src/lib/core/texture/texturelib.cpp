#include "texturelib.h"

#include <SDL_image.h>

namespace pgn
{
	cTextureLib::~cTextureLib()
	{
		for (auto kv : mTextures)
			SDL_DestroyTexture(kv.first);
	}

	void cTextureLib::Store(const cTexture& tex)
	{
		mTextures.insert(tex);
	}

	cTexture cTextureLib::Load(const char * fname, const char * desc)
	{
		SDL_Texture * tex = IMG_LoadTexture(mRenderer, fname);
		std::string name = desc ? desc : "";
		cTexture ctex = std::make_pair(tex, name);
		if (tex)
			mTextures.insert(ctex);
		return ctex;
	}

	cTextureLib::texture_set_type::iterator cTextureLib::FindByName(const std::string& s)
	{
		return std::find(mTextures.begin(), mTextures.end(), [&](const cTexture& t) { return t.second == s; });
	}

	cTextureLib::texture_set_type::iterator cTextureLib::FindByTexture(SDL_Texture * tex)
	{
		return std::find(mTextures.begin(), mTextures.end(), [&](const cTexture& t) { return t.first == tex; });
	}

	void cTextureLib::Unload(const std::string& desc)
	{
		auto it = FindByName(desc);
		if (it != mTextures.end())
		{
			auto tex = it->first;
			mTextures.erase(it);
			SDL_DestroyTexture(tex);
		}
	}
}