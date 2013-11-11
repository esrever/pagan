#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace pgn
{
	namespace curses
	{
		class cFontPreset
		{
		public:
			static void Add(const std::wstring& zName, 
							bool zIsBold,
							const glm::vec3& zFront, 
							const glm::vec3& zBack) ;

			static unsigned short Get(const std::wstring& zName) ;

		private:
			// set  
			static std::map<std::wstring, unsigned short> mFontPresets;
		};
	}
}