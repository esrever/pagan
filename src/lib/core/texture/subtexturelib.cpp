#include "subtexturelib.h"

#include <core/serialize/serialize.h>
#include <core/sdl/util.h>

#include "SDL_rect.h"

namespace pgn
{
	void cSubTextureLib::AddRect(const std::string& s, const SDL_Rect & r)
	{
		mRects[s] = r;
	}

	void cSubTextureLib::AddRect(const SDL_Rect & r)
	{
		mRects[std::to_string(r)] = r;
	}

	const SDL_Rect * cSubTextureLib::Rect(const std::string& s) const
	{
		auto it = mRects.find(s);
		if (it != mRects.end())
			return &it->second;
		else
			return nullptr;
	}
}