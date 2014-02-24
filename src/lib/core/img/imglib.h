#pragma once

#include <map>

struct SDL_Texture;
struct SDL_Renderer;

namespace pgn
{
	class cImgLib
	{
		public:
			cImgLib(SDL_Renderer * renderer) :mRenderer(renderer){}
			~cImgLib();
			void Store(SDL_Texture * tex, const char * desc);
			SDL_Texture * Load(const char * fname, const char * desc = nullptr);
			void Unload(const char * desc);
		private:
			std::map<std::string, SDL_Texture *> mTextures;
			SDL_Renderer * mRenderer;
	};
}