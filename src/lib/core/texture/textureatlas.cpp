#include "textureatlas.h"

#include <algorithm>
#include <pugixml.hpp>

#include "texturelib.h"

namespace pgn
{
	std::string cTextureAtlas::ParseDesc(const std::string& zDescName)
	{
		//mTileRes.loadXML(zResName);
		//auto tilemap = mTileRes.getResAnim("tilemap");

		std::string ret;
		pugi::xml_document doc;
		if (doc.load_file(zDescName.c_str(), pugi::parse_default | pugi::parse_comments))
		{
			auto imgnode = doc.child("image");
			auto fname = imgnode.attribute("file").as_string();
			ret = fname;
			mTileSize.x = imgnode.attribute("frame_width").as_uint();
			mTileSize.y = imgnode.attribute("frame_height").as_uint();

			//[code_modify_base_node
			mDims.x = mDims.y = 0;
			for (auto it : doc.child("desc"))
			{
				auto name = it.attribute("name").as_string();
				auto row = it.attribute("row").as_uint();
				auto col = it.attribute("col").as_uint();
				mDims.x = std::max(mDims.x, col);
				mDims.y = std::max(mDims.y, row);
				glm::ivec2 idx(col, row);
				mSpritePositions[name] = idx;
			}
			mDims = mDims + glm::uvec2(1, 1);
		}
		return ret;
	}

	cSubTexture cTextureAtlas::SubTexture(const std::string& name) const
	{
		//auto it = 
		//return cSubTexture(Texture(), *SpritePosMap().find(name));
		return cSubTexture();
	}

	cSubTexture cTextureAtlas::SubTexture(size_t x, size_t y) const
	{
		return cSubTexture();
	}
}