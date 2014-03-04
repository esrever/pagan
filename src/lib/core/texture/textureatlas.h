#pragma once

#include <functional>
#include <map>
#include <glm/glm.hpp>

#include "subtexturelib.h"

namespace pgn
{
	class cTextureAtlas : public cSubTextureLib
	{
	public:
		typedef std::map<std::string, glm::ivec2> SpritePosMap;

		std::string Init(const std::string& fname);

		const glm::uvec2& Dims() const				{ return mDims; }
		const glm::uvec2& TileSize() const			{ return mTileSize; }
	private:

		glm::uvec2 mTileSize;
		glm::uvec2 mDims;

	};

	typedef std::shared_ptr<cTextureAtlas> cTextureAtlas_sptr;

}