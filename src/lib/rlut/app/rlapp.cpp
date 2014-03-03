#include "rlapp.h"

#include <algorithm>
#include <core/app/sdlwin.h>
#include <core/sdl/font.h>

namespace pgn
{
	namespace rlut
	{
		void cRlApp::Init()
		{
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


			MainWindow()->TextureLib()->
			mSpriteAtlas.Init(TextureLib(), "C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tiledesc.xml");

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
			for (size_t i = 0; i < mGridDims.y; ++i)
			for (size_t j = 0; j < mGridDims.x; ++j)
			{
				size_t o = mDungeon.mMapData(j, i) & (pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor | pgn::rlut::eMapData::conn)
					? 160 : 191;
				size_t tgtx = o % mSpriteAtlas.Dims().x;
				size_t tgty = (o / mSpriteAtlas.Dims().x) % mSpriteAtlas.Dims().y;
				auto sprite = mSpriteAtlas.GetSprite(tgtx, tgty);
				SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };

				MainWindow()->RenderEx(sprite.first, { 255, 255, 255, 255 }, &sprite.second, &rect);
			}

			pgn::cSDLFont font(MainWindow()->Renderer(), "c:\\Windows\\fonts\\DejaVuSerif.ttf", 32);

			SDL_Rect textRect;
			auto tex = font.CreateText("Hello world!", &textRect);

			float ar = textRect.w / float(textRect.h);

			for (size_t i = 0; i < mNumLines; ++i)
			{
				size_t yo = mLogStart.y + i*msTextHeight;
				SDL_Rect rect = { mLogStart.x, yo, int(ar * msTextHeight), msTextHeight };
				MainWindow()->Render(tex.get(), &rect);
			}
		}
	}
}