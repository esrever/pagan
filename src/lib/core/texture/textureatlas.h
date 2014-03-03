#pragma once

#include <map>

#include <glm/glm.hpp>

#include "texture.h"

namespace pgn
{
	class cTextureLib;
	class cTextureAtlas : public cTexture
	{
	public:
		typedef std::map<std::string, glm::ivec2> SpritePosMap;

		// Parse the desc file for the actual texture filename
		std::string ParseDesc(const std::string& zDescName);

		cSubTexture SubTexture(const std::string& name) const;
		cSubTexture SubTexture(size_t x, size_t y) const;

		const SpritePosMap& SpritePositions() const	{ return mSpritePositions; }
		const glm::uvec2& Dims() const				{ return mDims; }
		const glm::uvec2& TileSize() const			{ return mTileSize; }
	private:

		SpritePosMap mSpritePositions;
		glm::uvec2 mTileSize;
		glm::uvec2 mDims;

	};
}