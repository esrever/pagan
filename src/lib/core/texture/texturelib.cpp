#include "texturelib.h"

#include <algorithm>

#include <pystring.h>
#include <SDL_image.h>

namespace pgn
{
	cTextureLib::~cTextureLib()
	{
		for (auto kv : mTextures)
			SDL_DestroyTexture(kv->Texture());
	}

	void cTextureLib::Store(cTexture_sptr tex)
	{
		mTextures.insert(tex);
	}

	cTexture_sptr cTextureLib::Load(const char * fname, const char * desc)
	{
		texture_loadfunc_type func = [&](const std::string& s){return IMG_LoadTexture(mRenderer, s.c_str()); };
		cTexture_sptr ptex;
		if (pystring::endswith(fname,".xml"))
		{
			ptex = cTextureAtlas_sptr(new cTextureAtlas());
		}
		else
		{
			ptex = cTexture_sptr(new cTexture());
		}
		ptex->Load(func, fname, desc);
		if (ptex->Texture())
			Store(ptex);
		return ptex;
	}

	cTexture_sptr cTextureLib::FindByName(const std::string& s) const
	{
		auto it = std::find_if(mTextures.begin(), mTextures.end(), [&](const cTexture_sptr& t) { return bool(t->Name() == s); });
		return it == mTextures.end() ? nullptr : *it;
	}

	cTexture_sptr cTextureLib::FindByTexture(SDL_Texture * tex) const
	{
		auto it = std::find_if(mTextures.begin(), mTextures.end(), [&](const cTexture_sptr& t) { return bool(t->Texture() == tex); });
		return it == mTextures.end() ? nullptr : *it;
	}

	void cTextureLib::Unload(const std::string& desc)
	{
		auto ptr = FindByName(desc);
		if (ptr)
		{
			auto tex = ptr->Texture();
			mTextures.erase(ptr);
			SDL_DestroyTexture(tex);
		}
	}

	const cTextureAtlas_sptr cTextureLib::Atlas(const std::string& name) const
	{
		return std::dynamic_pointer_cast<cTextureAtlas>(FindByName(name));
	}
}