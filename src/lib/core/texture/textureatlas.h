#pragma once

#include <map>

#include <glm/glm.hpp>

#include "texture.h"

namespace pgn
{
	class cTextureLib;
	class cTextureAtlas
	{
	public:
		typedef std::map<std::string, glm::ivec2> SpritePosMap;

		cTextureAtlas(cTextureLib * lib) :mTextureLib(lib) {}

		// Load xml file with all tiles and descriptors
		void Init(const std::string& zDescName);

		cSprite GetSprite(const std::string& name);
		cSprite GetSprite(size_t x, size_t y);

		const SpritePosMap& SpritePositions() const	{ return mSpritePositions; }
		const glm::uvec2& Dims() const				{ return mDims; }
		const glm::uvec2& TileSize() const			{ return mTileSize; }
	private:
		cTextureLib * mTextureLib;
		cTexture mTexture;

		SpritePosMap mSpritePositions;
		glm::uvec2 mTileSize;
		glm::uvec2 mDims;

	};
}