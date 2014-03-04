#pragma once

#include <map>
#include <memory>
#include <string>

struct SDL_Rect;

namespace pgn
{
	class cSubTextureLib
	{
		public:
			virtual ~cSubTextureLib(){}
			void AddRect(const std::string& s, const SDL_Rect & r);
			void AddRect(const SDL_Rect & r);

			const SDL_Rect * Rect() const { return nullptr; }
			const SDL_Rect * Rect(const std::string& s) const;
			const std::map<std::string, SDL_Rect>& Rects() const { return mRects; }
			
		private:
			std::map<std::string, SDL_Rect> mRects;
	};

	typedef std::shared_ptr<cSubTextureLib> cSubTextureLib_sptr;
}