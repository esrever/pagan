#pragma once

#include <utility>
#include <string>

#include <SDL.h>

namespace pgn
{
	class cTexture;
	typedef std::pair<cTexture, const SDL_Rect> cSubTexture;

	class cTexture
	{
		public:
			cTexture(SDL_Texture * tex = nullptr, const std::string& name = "") :mTexture(tex), mName(name){}
			virtual ~cTexture(){}

			const std::string& Name() const { return mName; }
			SDL_Texture * Texture() const { return mTexture; }
			cSubTexture SubTexture(const SDL_Rect * rect = nullptr) const;

			inline bool operator < (const cTexture& t) const
			{
				return std::make_pair(mTexture, mName) < std::make_pair(t.mTexture, t.mName);
			}
			
		private:
			SDL_Texture * mTexture;
			std::string mName;
	};
}