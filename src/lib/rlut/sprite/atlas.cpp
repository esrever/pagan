#include "atlas.h"

#include <algorithm>
#include <pugixml/pugixml.hpp>

#include <SDL_image.h>

#include <core/texture/texturelib.h>

namespace pgn
{
	namespace rlut
	{
		void cSpriteAtlas::Init(cTextureLib * lib, const std::string& zDescName)
		{
			//mTileRes.loadXML(zResName);
			//auto tilemap = mTileRes.getResAnim("tilemap");

			pugi::xml_document doc;
			if (doc.load_file(zDescName.c_str(), pugi::parse_default | pugi::parse_comments))
			{
				auto imgnode = doc.child("image");
				auto fname = imgnode.attribute("file").as_string();
				mTexture = lib->Load(fname);
				mTileSize = imgnode.attribute("frame_size").as_uint();

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

		cSprite cSpriteAtlas::GetSprite(const std::string& name)
		{
			auto it = mSpritePositions.find(name);
			if (it != mSpritePositions.end())
			{
				const auto& idx = it->second;
				return GetSprite(idx.x, idx.y);
			}
			else
				return cSprite(nullptr, {0,0,0,0});
		}

		cSprite cSpriteAtlas::GetSprite(size_t x, size_t y)
		{
			SDL_Rect rect = { x * mTileSize, y * mTileSize, mTileSize, mTileSize };
			return cSprite(mTexture, rect);
		}
	}
}