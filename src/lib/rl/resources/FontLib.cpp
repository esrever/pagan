#include "FontLib.h"

namespace pgn
{
	oxygine::Font * cFontLib::Get(const std::string& zFontName)
	{
		return mFontResources.getResFont(zFontName)->getFont();
	}

	void cFontLib::Init(const std::string& zResFile)
	{
		mFontResources.loadXML(zResFile);
	}
	
}