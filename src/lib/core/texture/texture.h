#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <string>

#include <SDL.h>

namespace pgn
{
	class cTexture;
	typedef std::shared_ptr<cTexture> cTexture_sptr;
	typedef std::pair<cTexture*, const SDL_Rect> cSubTexture;
	typedef std::function<SDL_Texture *(const std::string&)> texture_loadfunc_type;

	class cTexture
	{
		public:
			cTexture(SDL_Texture * tex = nullptr, const std::string& name = "") :mTexture(tex), mName(name){}
			virtual void Load(texture_loadfunc_type func, const std::string& fname, const char * desc = nullptr);
			virtual ~cTexture(){}

			const std::string& Name() const { return mName; }
			SDL_Texture * Texture() const { return mTexture; }
			cSubTexture SubTexture(const SDL_Rect * rect = nullptr);

			inline bool operator < (const cTexture& t) const
			{
				return std::make_pair(mTexture, mName) < std::make_pair(t.mTexture, t.mName);
			}
			
		private:
			SDL_Texture * mTexture;
			std::string mName;
	};
}