#include <iostream>
#include <algorithm>

#include <pystring.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>

#include <core/app/sdlapp.h>
#include <core/app/sdlwin.h>
#include <core/sdl/font.h>

#include <glm/glm.hpp>

#include <rlut/sprite/atlas.h>

struct cTestApp : public pgn::cSDLApp
{
	cTestApp(int argc, char ** argv) : pgn::cSDLApp(argc, argv)
	{

	}

	//------------------------------------------------
	virtual void Init()
	{
		static const glm::uvec2 gridDims(40,20);
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
	}

	//------------------------------------------------
	virtual void Render()
	{
		for (size_t i = 0; i < mGridDims.y;++i)
		for (size_t j = 0; j < mGridDims.x; ++j)
		{
			//SDL_Color c = { j % 256, i % 256, 0, 255 };
			/*
			SDL_Color c = { rand() % 256, rand() % 256, rand() % 256, 255 };
			SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };
			MainWindow()->Render(c,&rect);
			*/
			size_t o = i*mGridDims.x + j;
			size_t tgtx = o % mSpriteAtlas.Dims().x;
			size_t tgty = (o / mSpriteAtlas.Dims().x) % mSpriteAtlas.Dims().y;
			auto sprite = mSpriteAtlas.GetSprite(tgtx, tgty);
			SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };
			MainWindow()->RenderEx(sprite.first, {255,255,255,255}, &sprite.second, &rect);
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