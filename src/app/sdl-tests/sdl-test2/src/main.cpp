#include <iostream>
#include <algorithm>

#include <pystring.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>

#include <core/app/sdlapp.h>
#include <core/app/sdlwin.h>
#include <core/sdl/font.h>
#include <core/math/norm.h>

#include <glm/glm.hpp>

#include <rlut/sprite/atlas.h>
#include <rlut/dungen/dungen.h>
#include <rlut/path/difi.h>

#include <core/serialize/serialize.h>

struct cTestApp : public pgn::cSDLApp
{
	cTestApp(int argc, char ** argv) : pgn::cSDLApp(argc, argv)
	{

	}

	//------------------------------------------------
	virtual void Init()
	{
		static const glm::uvec2 gridDims(80,40);
		static const size_t  numLines = 4;

		mGridDims = gridDims;
		mNumLines = numLines;
		glm::ivec2 windims(MainWindow()->Rect().w, MainWindow()->Rect().h);

		const size_t logsize = mNumLines * msTextHeight + msTextHeight/4;
		int tile_h = (windims.y - logsize) / mGridDims.y; 
		int tile_w = windims.x / mGridDims.x;
		mTileDim = std::min(tile_w, tile_h);

		mGridStart = glm::uvec2(0, 0);
		mLogStart = glm::uvec2(5, mTileDim*mGridDims.y + msTextHeight / 4);
		mStatusStart = glm::uvec2(5+mTileDim*mGridDims.x, 0);


		mSpriteAtlas.Init(MainWindow()->ImgLib(), "C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tiledesc.xml");

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

#if 0
		mDiFi.Init(glm::ivec2(mGridDims.x, mGridDims.y));
		auto mfunc = [&](const glm::ivec2& p0, const glm::ivec2& p1){ 
			return mDungeon.mMapData(p1) & (pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor | pgn::rlut::eMapData::conn) ? pgn::norm_2(p0-p1) : std::numeric_limits<float>::max();
		};
		mDiFi.Generate(mfunc, glm::ivec2(35,30));
#else
		// DiFi 9,9 is centered at 35,30:
		mDiFi.Init(glm::ivec2(17, 17));
		auto mfunc = [&](const glm::ivec2& p0, const glm::ivec2& p1){
			if (mDungeon.mMapData.InRange(p1))
				return mDungeon.mMapData(p1) & (pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor | pgn::rlut::eMapData::conn) ? pgn::norm_2(p0 - p1) : std::numeric_limits<float>::max();
			else
				return std::numeric_limits<float>::max();
		};
		mDiFi.Generate(mfunc, glm::ivec2(26,21), glm::ivec2(35, 30));
#endif
	}

	//------------------------------------------------
	virtual void Render()
	{
		for (size_t i = 0; i < mGridDims.y;++i)
		for (size_t j = 0; j < mGridDims.x; ++j)
		{
			size_t o = mDungeon.mMapData(j, i) & (pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor | pgn::rlut::eMapData::conn)
				? 160 : 191;
			size_t tgtx = o % mSpriteAtlas.Dims().x;
			size_t tgty = (o / mSpriteAtlas.Dims().x) % mSpriteAtlas.Dims().y;
			auto sprite = mSpriteAtlas.GetSprite(tgtx, tgty);
			SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };

			glm::ivec2 pd = glm::ivec2(j, i) - mDiFi.CornerWcs();

			int v = ((!mDiFi.Data().InRange(pd)) || (mDiFi.Data()(pd)) == std::numeric_limits<float>::max()) 
				? 140 
				: std::max(140, 255 - int(10 * mDiFi.Data()(pd))); 
			MainWindow()->RenderEx(sprite.first, {v,v,v,255}, &sprite.second, &rect);
		}

		pgn::cSDLFont font(MainWindow()->Renderer(), "c:\\Windows\\fonts\\DejaVuSerif.ttf", 32);

		SDL_Rect textRect;
		auto tex = font.CreateText("Hello world!",&textRect);

		float ar = textRect.w / float(textRect.h);

		for (size_t i = 0; i < mNumLines; ++i)
		{
			size_t yo = mLogStart.y + i*msTextHeight;
			SDL_Rect rect = { mLogStart.x, yo, int(ar * msTextHeight), msTextHeight };
			MainWindow()->Render(tex.get(), &rect);
		}
	}
	//------------------------------------------------
	virtual bool Update()
	{

		return true;
	}
	//------------------------------------------------
	virtual void Destroy(){}

	//------------------------------------------------
	glm::uvec2 mGridDims;
	size_t	   mNumLines;
	pgn::rlut::cSpriteAtlas mSpriteAtlas;
	
	pgn::rlut::cWorkspace mDungeon;
	pgn::rlut::cDiFi	  mDiFi;

	size_t	   mTileDim;
	glm::uvec2 mGridStart;
	glm::uvec2 mLogStart;
	glm::uvec2 mStatusStart;
	
	static const size_t	msTextHeight = 20;
};

int main(int argc, char ** argv)
{
	std::string exepath = argv[0];
	exepath = pystring::replace(exepath, "/", "\\");
	std::vector<std::string> chunks;
	pystring::rsplit(exepath, chunks, "\\", 1);
	exepath = chunks[0] + "\\";

	cTestApp app(argc, argv);
	app.Run();
}