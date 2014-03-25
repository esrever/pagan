#include "RenderGameMap.h"

#include <core/texture/texturelib.h>
#include <core/texture/subtexturelib.h>

#include <rl/event/events.h>
#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cRenderGameMap::operator()()
			{
				auto& ecs = pgn::mainecs();
				// get hero and level entities
				auto hero = pgn::mainecs()->TagusToEntities("Player");
				auto lvl = pgn::mainecs()->TagusToEntities("CurrentLevel");

				// get hero loc and level data
				auto& hero_pos = hero->second->second.Component<pgn::ecs::cmp::cLocation>()->mPos;
				auto hero_vis = hero->second->second.Component<pgn::ecs::cmp::cVisibility>();
				auto& lvl_layout = lvl->second->second.Component<pgn::ecs::cmp::cLevelData>()->mLayout;
				auto lvl_id = lvl->second->first;

				// get the renderrect
				auto view_size = glm::ivec2(mGridSize.x, mGridSize.y);
				auto view_start = pgn::rl::GetCenteredView(glm::ivec2(lvl_layout.Dims().x, lvl_layout.Dims().y),
					hero_pos,
					view_size);
				view_size.x = std::min(view_size.x, int(lvl_layout.Dims().x - view_start.x));
				view_size.y = std::min(view_size.y, int(lvl_layout.Dims().y - view_start.y));
				auto view_end = view_start + view_size;

				auto& lvl_bg = lvl_layout.Bg().Cells();
				auto& lvl_fg = lvl_layout.Fg().Cells();
				auto& lvl_act = lvl_layout.Actors();

				auto tex_atlas = mWindow->TextureLib()->FindByName();
				auto tex = tex_atlas->first;
				auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);

				auto& visview = hero_vis->mVisible[lvl_id].View();
				auto& expview = hero_vis->mExplored[lvl_id].View();
				std::function<int(int x, int y)> get_fow = [&](int x, int y){return visview(x, y) ? 255 : (expview(x, y) ? 100 : 0); };

				RenderDense1(lvl_layout.Bg(), view_start, view_size, get_fow);
				RenderSparse1(lvl_layout.Fg(), view_start, view_size, get_fow);
				RenderSparse1(lvl_layout.Actors(), view_start, view_size, get_fow);

				return false;
			}

			void cRenderGameMap::RenderDense1(const rl::cTileStoreDense1& ts, const glm::ivec2& start, const glm::ivec2 size, std::function<int(int x, int y)> visfunc)
			{
				auto end = start + size;
				for (int y = start.y; y < end.y;++y)
					for (int x = start.x; x < end.x; ++x)
					{
						const auto pos = glm::ivec2(x, y);
						auto ed = ts.Cells()(pos);
						RenderTile(ed, pos, pos-start, visfunc(x, y));
					}
			}

			void cRenderGameMap::RenderSparse1(const rl::cTileStoreSparse1& ts, const glm::ivec2& start, const glm::ivec2 size, std::function<int(int x, int y)> visfunc)
			{
				cArrayShape2D shape(size.x, size.y, start.x, start.y);
				if (ts.Entities().empty())
					return;
				for (const auto& ed : ts.Entities())
				{
					// make the FoW non-renders a bit clearer
					auto pos = ed->second.Component<cmp::cLocation>()->mPos;
					auto ivis = visfunc(pos.x, pos.y);
					if (ed->second.Component<cmp::cControllerAI>() && (ivis < 255))
						continue;
					
					// TODO: static inrange func! or inrect or sth
					if ( shape.InRange(pos.x, pos.y))
						RenderTile(ed, pos, pos-start, ivis);
				}
			}

			void cRenderGameMap::RenderTile(ecs::cEntityWithDataConst ed, const glm::ivec2& pos, const glm::ivec2& offpos, int ivis)
			{
				auto sspos = offpos;
				sspos.y = mGridSize.y - 1 - sspos.y;
				sspos += mGridStart;
				auto texset = ed->second.Component<cmp::cTextureSet>();
				pgn::cSubTexture tex = texset->mSprites[texset->mIndex];
				SDL_Rect tgt_rect = { sspos.x*mTileSize, sspos.y*mTileSize, mTileSize, mTileSize };
				auto src_rect = tex.second;
				SDL_Color col = { ivis, ivis, ivis , 255 };
				mWindow->RenderEx(tex.first->Texture(), col, &src_rect, &tgt_rect);
			}

			void cRenderGameMap::SetArea(const glm::uvec2& start, const glm::uvec2& dims, size_t tileDim)
			{
				mGridStart = start;
				mGridSize = dims;
				mTileSize = tileDim;
			}
		}
	}
}