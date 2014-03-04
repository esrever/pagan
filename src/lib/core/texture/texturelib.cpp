#include "texturelib.h"

#include <algorithm>

#include <pystring.h>
#include <SDL_image.h>

namespace pgn
{
	cTextureLib::~cTextureLib()
	{
		for (auto kv : mTextures)
			SDL_DestroyTexture(kv.first->Texture());
	}

	cTextureLib::textures_type::iterator cTextureLib::Store(cTexture_sptr tex)
	{
		return mTextures.insert( std::make_pair(tex,nullptr)).first;
	}

	cTexture_sptr cTextureLib::Load(const char * cfname, const char * desc)
	{
		std::string fname = cfname;
		texture_loadfunc_type func = [&](const std::string& s){return IMG_LoadTexture(mRenderer, s.c_str()); };
		
		cTextureAtlas_sptr atex;
		if (pystring::endswith(fname,".xml"))
		{
			atex = cTextureAtlas_sptr(new cTextureAtlas());
			fname = atex->Init(fname);
		}
		cTexture_sptr ptex( new cTexture(IMG_LoadTexture(mRenderer, fname.c_str()), desc ? desc : fname));
		if (ptex->Texture())
			Store(ptex)->second = atex;
		return ptex;
	}

	cTextureLib::textures_type::const_iterator cTextureLib::FindByName(const std::string& s) const
	{
		return std::find_if(mTextures.begin(), mTextures.end(), [&](const textures_type::value_type& t) { return bool(t.first->Name() == s); });
	}

	void cTextureLib::Unload(const std::string& desc)
	{
		auto ptr = FindByName(desc);
		if (ptr != mTextures.end())
		{
			auto tex = ptr->first->Texture();
			mTextures.erase(ptr);
			SDL_DestroyTexture(tex);
		}
	}
}