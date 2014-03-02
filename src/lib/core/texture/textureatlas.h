#pragma once

namespace pgn
{
#pragma once

#include <map>

#include <glm/glm.hpp>

#include "texture.h"

	namespace pgn
	{
		class cTextureLib;
		namespace rlut
		{
			class cSpriteAtlas
			{
			public:
				typedef std::map<std::string, glm::ivec2> SpritePosMap;

				cSpriteAtlas(const std::string& atlasName = "") :mName(atlasName){}

				// Load xml file with all tiles and descriptors
				void Init(cImgLib * lib, const std::string& zDescName);

				const SpritePosMap& GetMap() { return mSpritePositions; }
				cSprite GetSprite(const std::string& name);
				cSprite GetSprite(size_t x, size_t y);
				const glm::uvec2& Dims() const { return mDims; }
			private:
				std::string mName;
				SDL_Texture * mTexture;
				SpritePosMap mSpritePositions;
				size_t mTileSize;
				glm::uvec2 mDims;

			};
		}
	}
}