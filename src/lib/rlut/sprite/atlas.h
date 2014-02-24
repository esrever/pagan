#pragma once

#include <map>

#include <glm/glm.hpp>

#include "sprite.h"

namespace pgn
{
	class cImgLib; 
	namespace rlut
	{
		class cSpriteAtlas
		{
		public:
			typedef std::map<std::string, glm::ivec2> SpritePosMap;

			// Load xml file with all tiles and descriptors
			void Init(cImgLib * lib, const std::string& zDescName);

			const SpritePosMap& GetMap() { return mSpritePositions; }
			cSprite GetSprite(const std::string& name);
			cSprite GetSprite( size_t x, size_t y);
			const glm::uvec2& Dims() const { return mDims; }
		private:
			
			SDL_Texture * mTexture;
			SpritePosMap mSpritePositions;
			size_t mTileSize;
			glm::uvec2 mDims;
			
		};
	}
}