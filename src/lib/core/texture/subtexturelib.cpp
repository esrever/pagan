#include "subtexturelib.h"

#include <core/serialize/serialize.h>
#include <core/sdl/util.h>

namespace pgn
{
	void cSubTextureLib::AddRect(const std::string& s, const cBox2i & r)
	{
		mRects[s] = r;
	}

	void cSubTextureLib::AddRect(const cBox2i & r)
	{
		mRects[std::to_string(r)] = r;
	}

	const cBox2i * cSubTextureLib::Rect(const std::string& s) const
	{
		auto it = mRects.find(s);
		if (it != mRects.end())
			return &it->second;
		else
			return nullptr;
	}
}