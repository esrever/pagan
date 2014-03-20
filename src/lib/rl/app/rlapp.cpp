#include "rlapp.h"

#include <algorithm>
#include <core/app/sdlwin.h>
#include <core/sdl/font.h>

#include <core/texture/texturelib.h>

#include <rl/ai/actions.h>
#include <rl/ai/conditions.h>
#include <rl/components/RegisterComponents.h>
#include <rl/event/events.h>
#include <rl/event/actions.h>

#include <ecs/systems/InputKey.h>
#include <rl/systems/CalcVis.h>
#include <rl/systems/CreateLevel.h>
#include <rl/systems/GameTurn.h>

namespace pgn
{
	namespace rl
	{
		void cRlApp::Init()
		{
			// TODO: somewhere else? configurable?
			srand(0);

			// Initialize AI-related
			pgn::bt::RegisterActions();
			pgn::bt::RegisterConditions();
			pgn::ecs::RegisterComponents(*mainecs());
			pgn::rl::RegisterActions(*mainecs());

			// TODO: move system init elsewhere
			pgn::mainecs()->System<ecs::sys::cInputKey>();
			pgn::mainecs()->System<ecs::sys::cCalcVis>();
			pgn::mainecs()->System<ecs::sys::cCreateLevel>();
			pgn::mainecs()->System<pgn::ecs::sys::cGameTurn>();

			static const glm::uvec2 gridDims(40, 20);
			static const size_t  numLines = 4;

			mGridDims = gridDims;
			mNumLines = numLines;
			glm::ivec2 windims(MainWindow()->Rect().w, MainWindow()->Rect().h);

			const size_t logsize = mNumLines * msTextHeight + msTextHeight / 4;
			int tile_h = (windims.y - logsize) / mGridDims.y;
			int tile_w = windims.x / mGridDims.x;
			mTileDim = std::min(tile_w, tile_h);

			mGridStart = glm::uvec2(0, 0);
			mLogStart = glm::uvec2(5, mTileDim*mGridDims.y + msTextHeight / 4);
			mStatusStart = glm::uvec2(5 + mTileDim*mGridDims.x, 0);

			const char * fname_atlas = PROJECT_ROOT "data\\tiledesc.xml";
			MainWindow()->TextureLib()->Load(fname_atlas, "");
		}

		void cRlApp::Render()
		{
			auto tex_atlas = MainWindow()->TextureLib()->FindByName(":");
			auto tex = tex_atlas->first;
			auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);
			auto it_rect = atlas->Rects().cbegin();
			for (size_t i = 0; i < mGridDims.y; ++i)
			for (size_t j = 0; j < mGridDims.x; ++j)
			{
				size_t o = mDungeon.mMapData(j, i) & (pgn::rl::eMapData::room | pgn::rl::eMapData::corridor | pgn::rl::eMapData::conn)
					? 160 : 191;
				size_t tgtx = o % atlas->Dims().x;
				size_t tgty = (o / atlas->Dims().x) % atlas->Dims().y;
				SDL_Rect texrect = it_rect->second;
				++it_rect;
				SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };

				MainWindow()->RenderEx(tex->Texture(), { 255, 255, 255, 255 }, &texrect, &rect);
			}

			pgn::cSDLFont font(MainWindow()->Renderer(), "c:\\Windows\\fonts\\DejaVuSerif.ttf", 32);

			SDL_Rect textRect;
			auto texf = font.CreateText("Hello world!", &textRect);

			float ar = textRect.w / float(textRect.h);

			for (size_t i = 0; i < mNumLines; ++i)
			{
				size_t yo = mLogStart.y + i*msTextHeight;
				SDL_Rect rect = { mLogStart.x, yo, int(ar * msTextHeight), msTextHeight };
				MainWindow()->Render(texf.get(), &rect);
			}
		}
	}
}