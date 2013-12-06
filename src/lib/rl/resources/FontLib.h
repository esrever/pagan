#pragma once

#include <oxygine-framework.h>

namespace pgn
{
	class cFontLib
	{
	public:
		oxygine::Font * Get(const std::string& zFontName); 
		void Init(const std::string& zResFile);
	private:
		oxygine::Resources mFontResources;
	};
}