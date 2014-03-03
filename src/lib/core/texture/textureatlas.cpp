#include "textureatlas.h"

#include <algorithm>
#include <pugixml.hpp>

#include "texturelib.h"

namespace pgn
{
	void cTextureAtlas::Load(texture_loadfunc_type func, const std::string& fname, const char * desc)
	{
		//mTileRes.loadXML(zResName);
		//auto tilemap = mTileRes.getResAnim("tilemap");

		std::string ret;
		pugi::xml_document doc;
		if (doc.load_file(fname.c_str(), pugi::parse_default | pugi::parse_comments))
		{
			auto imgnode = doc.child("image");
			auto texfname = imgnode.attribute("file").as_string();
			cTexture::Load(func, texfname, desc);
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
	}

	cSubTexture cTextureAtlas::SubTexture(const std::string& name) 
	{
		auto it = SpritePosMap().find(name);
		return SubTexture(it->second.x, it->second.y);
	}

	cSubTexture cTextureAtlas::SubTexture(size_t x, size_t y) 
	{
		assert(x < mDims.x);
		assert(y < mDims.y);
		SDL_Rect rect;
		rect.x = x*mTileSize.x;
		rect.y = y*mTileSize.y;
		rect.w = mTileSize.x;
		rect.h = mTileSize.y;
		return cSubTexture(this, rect);
	}
}