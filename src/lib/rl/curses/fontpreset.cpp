#include "fontpreset.h"

#include <curses.h>
#include "color.h"

namespace pgn
{
	namespace curses
	{
		std::map<std::wstring, unsigned short> cFontPreset::mFontPresets;

		//------------------------------------------------------------------
		void cFontPreset::Add(const std::wstring& zName,
							  bool zIsBold,
							  const glm::vec3& zFront,
							  const glm::vec3& zBack) 
		{
			auto ci = cColor::CreateColorPairIndex(zFront, zBack) << (PDC_COLOR_SHIFT-16);
			if (zIsBold)
				ci |= A_BOLD>>16;
			mFontPresets[zName] = ci;
		}

		//------------------------------------------------------------------
		unsigned short cFontPreset::Get(const std::wstring& zName) 
		{
			auto it = mFontPresets.find(zName);
			if (it != mFontPresets.end())
				return it->second;
			else
				return 0;
		}
	}
}