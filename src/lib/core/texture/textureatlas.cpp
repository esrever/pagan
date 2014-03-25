#include "textureatlas.h"

#include <algorithm>
#include <pugixml.hpp>

#include <core/sdl/util.h>

#include "subtexturelib.h"

namespace pgn
{
	std::string cTextureAtlas::Init(const std::string& fname)
	{
		//mTileRes.loadXML(zResName);
		//auto tilemap = mTileRes.getResAnim("tilemap");

		std::string ret;
		pugi::xml_document doc;
		if (doc.load_file(fname.c_str(), pugi::parse_default | pugi::parse_comments))
		{
			auto imgnode = doc.child("image");
			auto texfname = imgnode.attribute("file").as_string();
			mTileSize.x = imgnode.attribute("frame_width").as_uint();
			mTileSize.y = imgnode.attribute("frame_height").as_uint();

			mDims.x = mDims.y = 0;
			for (auto it : doc.child("desc"))
			{
				auto name = it.attribute("name").as_string();
				auto row = it.attribute("row").as_uint();
				auto col = it.attribute("col").as_uint();
				mDims.x = std::max(mDims.x, col);
				mDims.y = std::max(mDims.y, row);
				glm::ivec2 idx(col, row);
				SDL_Rect r = { col*mTileSize.x, row*mTileSize.y, mTileSize.x, mTileSize.y };
				AddRect(name, as_rect(r));
			}
			mDims = mDims + glm::uvec2(1, 1);

			return texfname;
		}
		return "";
	}
}