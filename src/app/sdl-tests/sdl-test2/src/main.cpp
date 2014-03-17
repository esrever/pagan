#include <iostream>
#include <algorithm>

#include <pystring.h>
#include <SDL_image.h>

#include <core/app/sdlapp.h>
#include <core/app/sdlwin.h>
#include <core/event/Event.h>
#include <core/sdl/font.h>
#include <core/math/norm.h>
#include <core/texture/texturelib.h>

#include <glm/glm.hpp>

#include <rlut/dungen/dungen.h>
#include <rlut/path/difi.h>
#include <rlut/fov/FovLookup.h>
#include <rlut/fov/fov_rsc.h>

#include <core/serialize/serialize.h>
#include <core/texture/texture.h>
#include <core/texture/texturelib.h>
#include <core/texture/subtexturelib.h>

struct cTestApp : public pgn::cSDLApp
{
	cTestApp(int argc, char ** argv) : pgn::cSDLApp(argc, argv),
	INIT_EVT_MEMBER(cTestApp, MouseMotion),
	INIT_EVT_MEMBER(cTestApp, Keyboard),
	mMouseOverCell(0,0),
	mLoS(10)
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
		mLogStart = glm::uvec2(15, mTileDim*mGridDims.y + msTextHeight / 4);
		mStatusStart = glm::uvec2(5+mTileDim*mGridDims.x, 0);

		MainWindow()->TextureLib()->Load("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tiledesc.xml","");

		auto atlas = Resources<pgn::cTextureLib>()->FindByName();
		mTextureFloor = pgn::cSubTexture(atlas->first, *atlas->second->Rect("dungeon:floor:rect_gray0"));
		mTextureWall = pgn::cSubTexture(atlas->first, *atlas->second->Rect("dungeon:wall:marble_wall1"));
		mTextureDoor = pgn::cSubTexture(atlas->first, *atlas->second->Rect("dungeon:dngn_closed_door"));

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
#endif
	}

	//------------------------------------------------
	virtual void Render()
	{
		//auto atlas = MainWindow()->TextureLib()->Atlas();
		auto tex_atlas = MainWindow()->TextureLib()->FindByName();
		auto tex = tex_atlas->first;
		auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);
		for (size_t i = 0; i < mGridDims.y;++i)
		for (size_t j = 0; j < mGridDims.x; ++j)
		{
			pgn::cSubTexture tex;
			if (mDungeon.mMapData(j, i) & pgn::rlut::eMapData::conn)
				tex = mTextureDoor;
			else if (mDungeon.mMapData(j, i) & (pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor))
				tex = mTextureFloor;
			else if (mDungeon.mMapData(j, i) & pgn::rlut::eMapData::perimeter)
				tex = mTextureWall;
			else
				continue;

			SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };

			glm::ivec2 pd = glm::ivec2(j, i) - mDiFi.CornerWcs();

			int v = ((!mDiFi.Data().InRange(pd)) || (mDiFi.Data()(pd)) == std::numeric_limits<float>::max()) 
				? 55 
				: std::max(55, 255 - int(200 * mDiFi.Data()(pd))); 
			// FOV:
			v = 50 + int(visf(j,i) * 205);
			//v = visf(j, i) > 0.0f ? 255 : 50;
			MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
		}

		pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\SourceSansPro\\SourceSansPro-Regular.otf", 32);
		//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\PT-Sans\\PTN57F.ttf", 62);
		//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\Nobile\\Nobile-Regular.ttf", 32);

		SDL_Rect textRect;
		char buf[256];
		sprintf(buf, "%d %d", mMouseOverCell.x, mMouseOverCell.y);
		auto texf = font.CreateText(buf,&textRect);

		float ar = textRect.w / float(textRect.h);

		for (size_t i = 0; i < mNumLines; ++i)
		{
			size_t yo = mLogStart.y + i*msTextHeight;
			SDL_Rect rect = { mLogStart.x, yo, int(ar * msTextHeight), msTextHeight };
			MainWindow()->Render(texf.get(), &rect);
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
	
	pgn::rlut::cWorkspace mDungeon;
	pgn::rlut::cDiFi	  mDiFi;

	size_t	   mTileDim;
	glm::uvec2 mGridStart;
	glm::uvec2 mLogStart;
	glm::uvec2 mStatusStart;

	glm::ivec2 mMouseOverCell;

	// FOV
	pgn::cArray2D<bool> vismap;
	pgn::cArray2D<float> visf;
	size_t mLoS;

	pgn::cSubTexture mTextureFloor;
	pgn::cSubTexture mTextureWall;
	pgn::cSubTexture mTextureDoor;

	void CalcFoV()
	{
		// FOV
		int visBits = pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor | pgn::rlut::eMapData::conn;
		pgn::rlut::cFovLookup<pgn::rlut::cFovRsc> flut;
		auto& fov = flut.Get(mLoS);
		auto& dmap = mDungeon.mMapData;
		vismap.Resize(dmap.Width(), dmap.Height());
		visf.Resize(dmap.Width(), dmap.Height(), 0.0f);
		visf.Fill(0.0f);
		std::vector<glm::ivec2> lospts;
		vismap.View().VisitWext([&](size_t x, size_t y, std::vector<bool>::reference v) {v = (dmap(x, y) & visBits) ? true : false; });

		auto onvis = [&](const glm::ivec2& pos, float vis) {lospts.push_back(pos); visf(pos) = vis; };
		fov.Calc(mMouseOverCell, vismap, onvis);
	}

	void CalcDiFi()
	{
		int r = mLoS;
		// DiFi of 10,10 radius
		mDiFi.Init(glm::ivec2(r * 2 + 1, r * 2 + 1));

		auto mfunc = [&](const glm::ivec2& p0, const glm::ivec2& p1){
			if (mDungeon.mMapData.InRange(p1))
				return mDungeon.mMapData(p1) & (pgn::rlut::eMapData::room | pgn::rlut::eMapData::corridor | pgn::rlut::eMapData::conn) ? pgn::norm_2(p0 - p1) : std::numeric_limits<float>::max();
			else
				return std::numeric_limits<float>::max();
		};

		// We feed the corner and the goal in world coordinate
		mDiFi.Generate(mfunc, mMouseOverCell - glm::ivec2(r, r), mMouseOverCell);

		auto& rawdata = mDiFi.Data().View().Storage().Raw();
		float minv = std::numeric_limits<float>::max();
		float maxv = std::numeric_limits<float>::min();
		for (const auto& v : rawdata)
		{
			if (v != std::numeric_limits<float>::max())
			{
				minv = std::min(minv, v);
				maxv = std::max(maxv, v);
			}
		}
		mDiFi.Data().View().VisitW([minv, maxv](float& v){ v = (v - minv) / (maxv - minv); });
		//std::cout << minv << " - " << maxv << std::endl;
	}

	//DECL_EVT_MEMBER(MouseMotion);
	void OnMouseMotion(const SDL_MouseMotionEvent& e)
	{
		mMouseOverCell.x = std::min( e.x / mTileDim, mGridDims.x-1);
		mMouseOverCell.y = std::min(e.y / mTileDim, mGridDims.y - 1);
		//mMouseOverCell.y = mGridDims.y - 1 - mMouseOverCell.y;

		CalcFoV();
		CalcDiFi();
	}

	void OnKeyboard(const SDL_KeyboardEvent& e)
	{
		if (e.state == 1)
		{
			if (e.keysym.scancode == SDL_SCANCODE_KP_PLUS)
			{
				mLoS++;
				CalcFoV();
				CalcDiFi();
			}
			else if (e.keysym.scancode == SDL_SCANCODE_KP_MINUS)
			{
				mLoS = std::max(size_t(1), size_t(mLoS - 1));
				CalcFoV();
				CalcDiFi();
			}
		}
	}

	DECL_EVT_MEMBER(MouseMotion);
	DECL_EVT_MEMBER(Keyboard);
	
	static const size_t	msTextHeight = 24;
};

int main(int argc, char ** argv)
{
	std::string exepath = argv[0];
	exepath = pystring::replace(exepath, "/", "\\");
	std::vector<std::string> chunks;
	pystring::rsplit(exepath, chunks, "\\", 1);
	exepath = chunks[0] + "\\";


	pgn::cSDLApp*& app = pgn::mainapp();
	app = new cTestApp(argc, argv);
	app->Run();
	delete app;
}