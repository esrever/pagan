#include "texturelib.h"

#include <algorithm>

#include <SDL_image.h>

namespace pgn
{
	cTextureLib::~cTextureLib()
	{
		for (auto kv : mTextures)
			SDL_DestroyTexture(kv.Texture());
	}

	void cTextureLib::Store(const cTexture& tex)
	{
		mTextures.insert(tex);
	}

	cTexture cTextureLib::Load(const char * fname, const char * desc)
	{
		SDL_Texture * tex = IMG_LoadTexture(mRenderer, fname);
		std::string name = desc ? desc : "";
		cTexture ctex = cTexture(tex, name);
		if (tex)
			mTextures.insert(ctex);
		return ctex;
	}

	cTextureLib::texture_set_type::iterator cTextureLib::FindByName(const std::string& s)
	{
		return std::find_if(mTextures.begin(), mTextures.end(), [&](const cTexture& t) { return bool(t.Name() == s); });
	}

	cTextureLib::texture_set_type::iterator cTextureLib::FindByTexture(SDL_Texture * tex)
	{
		return std::find_if(mTextures.begin(), mTextures.end(), [&](const cTexture& t) { return bool(t.Texture() == tex); });
	}

	void cTextureLib::Unload(const std::string& desc)
	{
		auto it = FindByName(desc);
		if (it != mTextures.end())
		{
			auto tex = it->Texture();
			mTextures.erase(it);
			SDL_DestroyTexture(tex);
		}
	}
}