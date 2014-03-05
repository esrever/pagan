#include <iostream>
#include <algorithm>

#include <pystring.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>

#include <core/app/sdlapp.h>
#include <core/app/sdlwin.h>
#include <core/sdl/font.h>
#include <core/texture/texturelib.h>

#include <glm/glm.hpp>

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

		MainWindow()->TextureLib()->Load("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tiledesc.xml","");
	}

	//------------------------------------------------
	virtual void Render()
	{
		auto tex_atlas = MainWindow()->TextureLib()->FindByName();
		auto tex = tex_atlas->first;
		auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);
		auto it_rect = atlas->Rects().cbegin();
		for (size_t i = 0; i < mGridDims.y;++i)
		for (size_t j = 0; j < mGridDims.x; ++j)
		{
			size_t o = i*mGridDims.x + j;
			
			if (it_rect != atlas->Rects().end())
			{
				SDL_Rect texrect = it_rect->second;
				SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };
				MainWindow()->RenderEx(tex->Texture(), { 255, 255, 255, 255 }, &texrect, &rect);
				++it_rect;
			}
		}

		pgn::cSDLFont font(MainWindow()->Renderer(), "c:\\Windows\\fonts\\DejaVuSerif.ttf", 32);

		SDL_Rect textRect;
		auto texf = font.CreateText("Hello world!",&textRect);

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

	pgn::cSDLApp*& app = pgn::mainapp();
	app = new cTestApp(argc, argv);
	app->Run();
	delete app;
}