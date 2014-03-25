#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <string>

#include <SDL.h>
#undef main

#include <core/container/box2.h>
#include <core/serialize/serialize.h>

namespace pgn
{
	class cTexture;
	typedef std::shared_ptr<cTexture> cTexture_sptr;
	typedef std::pair<cTexture_sptr, cBox2i> cSubTexture;

	class cTexture
	{
		public:
			cTexture(SDL_Texture * tex = nullptr, const std::string& name = "") :mTexture(tex), mName(name){}
			virtual ~cTexture(){}

			const std::string& Name() const { return mName; }
			SDL_Texture * Texture() const { return mTexture; }

			inline bool operator < (const cTexture& t) const
			{
				return std::make_pair(mTexture, mName) < std::make_pair(t.mTexture, t.mName);
			}

		public:
			static void SplitName(const std::string& name, std::string& lib, std::string& tex, std::string& subtex);
			
		private:
			SDL_Texture * mTexture;
			std::string mName;
	};

	DECL_SERIALIZE_INTERFACE(cSubTexture);

	//-------------------------------------------------------------
}

inline bool operator == (const SDL_Rect& lhs, const SDL_Rect& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(SDL_Rect)) == 0;
}