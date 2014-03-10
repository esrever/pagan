#include "rlapp.h"

#include <algorithm>
#include <core/app/sdlwin.h>
#include <core/sdl/font.h>

#include <core/texture/texturelib.h>

#include <rlut/ai/actions.h>
#include <rlut/ai/conditions.h>

namespace pgn
{
	namespace rlut
	{
		void cRlApp::Init()
		{
			// Initialize AI-related
			pgn::bt::RegisterActions();
			pgn::bt::RegisterConditions();

			static const glm::uvec2 gridDims(80, 40);
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

			//mSpriteAtlas.Init(TextureLib(), "C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tiledesc.xml");
			MainWindow()->TextureLib()->Load("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tiledesc.xml");

			srand(0);
			mDungeon.Init(mGridDims.x, mGridDims.y);
			mDungeon.mConstraints.RoomRectW(glm::ivec2(3, 12))
				.RoomRectH(glm::ivec2(3, 12))
				.DoorToDoorMinDistance(glm::ivec2(2, 2))
				.DoorsPerRoom(glm::ivec2(1, 4))
				.ConnectIslands(false)
				.StoreRoomConnectivity(true)
				.OverrideMaxConns(false)
				.RandomEntryExit(true);
			mDungeon.generate_dungeon(100);
		}

		void cRlApp::Render()
		{
			auto tex_atlas = MainWindow()->TextureLib()->FindByName("");
			auto tex = tex_atlas->first;
			auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);
			auto it_rect = atlas->Rects().cbegin();
			for (size_t i = 0; i < mGridDims.y; ++i)
			for (size_t j = 0; j < mGridDims.x; ++j)
			{
				size_t o = mDungeon.mMapData(j, i) & (pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor | pgn::rlut::eMapData::conn)
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