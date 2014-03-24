#include "RenderGameMap.h"

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

				auto& visview = hero_vis->mVisible[lvl_id].CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
				auto& expview = hero_vis->mExplored[lvl_id].CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
				std::function<int(int x, int y)> get_fow = [&](int x, int y){return visview(x, y) ? 255 : (expview(x, y) ? 100 : 0); };

				return false;
			}

			void cRenderGameMap::RenderTile(ecs::cEntityWithData ed, const glm::ivec2& pos, float vis)
			{
				auto texset = ed->second.Component<cmp::cTextureSet>();
				pgn::cSubTexture tex = texset->mSprites[texset->mIndex];
				auto ss_coord = ScreenPos(pos);
				auto tgt_rect = { ss_coord.x, ss_coord.y, mTileSize, mTileSize };
				auto src_rect = tex.second;
				Render(tex.first, tile_color, &src_rect, &tgt_rect)
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