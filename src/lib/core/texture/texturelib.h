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
			typedef std::set<cTexture> texture_set_type;
			cTextureLib(SDL_Renderer * renderer, const std::string& name = "") :mRenderer(renderer),mName(name){}
			~cTextureLib();
			void Store(const cTexture& tex);
			cTexture Load(const char * fname, const char * desc = nullptr);
			void Unload(const std::string& name);

			const cTextureAtlas * Atlas(const std::string& name="") const { auto it = mTextureAtlas.find(name);  return it == mTextureAtlas.end() ? &it->second : nullptr; }
			const texture_set_type Textures() const { return mTextures; }
			texture_set_type::iterator FindByName(const std::string& s);
			texture_set_type::iterator FindByTexture(SDL_Texture * tex);

			const std::string& Name() const { return mName; }
		private:
			std::string mName;
			std::set<cTexture> mTextures;
			std::map<std::string, cTextureAtlas> mTextureAtlas;
			SDL_Renderer * mRenderer;
	};
}