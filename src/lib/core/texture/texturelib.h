#pragma once

#include <set>
#include <map>

#include "texture.h"
#include "textureatlas.h"

struct SDL_Renderer;

namespace pgn
{
	class cTextureLib
	{
		public:
			typedef std::map< cTexture_sptr, cSubTextureLib_sptr> textures_type;

			cTextureLib(SDL_Renderer * renderer, const std::string& name = "") :mRenderer(renderer),mName(name){}
			~cTextureLib();

			textures_type::iterator Store(cTexture_sptr tex);
			cTexture_sptr Load(const char * fname, const char * desc = nullptr);
			void Unload(const std::string& name);
			
			textures_type::const_iterator FindByName(const std::string& s) const;

			const std::string& Name() const { return mName; }
		private:
			std::string		mName;
			textures_type	mTextures;
			SDL_Renderer *	mRenderer;
	};
}