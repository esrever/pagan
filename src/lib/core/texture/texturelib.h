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
			typedef std::set<cTexture_sptr> texture_set_type;
			cTextureLib(SDL_Renderer * renderer, const std::string& name = "") :mRenderer(renderer),mName(name){}
			~cTextureLib();
			void Store(cTexture_sptr tex);
			cTexture_sptr Load(const char * fname, const char * desc = nullptr);
			void Unload(const std::string& name);

			const cTextureAtlas_sptr Atlas(const std::string& name = "") const;
			const texture_set_type Textures() const { return mTextures; }
			
			cTexture_sptr FindByName(const std::string& s) const;
			cTexture_sptr FindByTexture(SDL_Texture * tex) const;

			const std::string& Name() const { return mName; }
		private:
			std::string mName;
			texture_set_type mTextures;
			SDL_Renderer * mRenderer;
	};
}