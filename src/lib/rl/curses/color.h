#pragma once

#include <cstdint>
#include <map>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace pgn
{
	namespace curses
	{
		class cColor
		{
		public:
			static uint16_t CreateColorPairIndex(const glm::vec3& zFront, const glm::vec3& zBack);
			static uint16_t CreateColorIndex(const glm::vec3& zCol);

		private:
			// set  
			static std::map<uint64_t, unsigned short> mColorToIndex;
			static std::map<uint32_t, unsigned short> mColorPairToIndex;
		};
	}
}