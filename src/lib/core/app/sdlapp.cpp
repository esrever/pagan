#include "sdlapp.h"

#include <stdexcept>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <core/texture/texturelib.h>

#include "sdlwin.h"

namespace pgn
{
	static cMain g_app = cMain();
	cSDLApp *& mainapp() { return g_app.mApp; }

	//----------------------------------------------------------------------
	cSDLApp::cSDLApp(int argc, char ** argv)
	{
		//initialize all SDL subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			throw std::runtime_error("SDL Init Failed");
		if (TTF_Init() == -1)
			throw std::runtime_error("TTF Init Failed");

		mMainWindow = std::shared_ptr<cSDLWindow>(new cSDLWindow());
	}

	//----------------------------------------------------------------------
	cSDLApp::~cSDLApp()
	{
		SDL_Quit();
	}

	//----------------------------------------------------------------------
	void cSDLApp::Run()
	{
		Init();
		bool ok = true;
		while (ok)
		{
			// messages
			ok = HandleEvents();
			ok = ok && Update();

			//Rendering
			SDL_RenderClear(mMainWindow->Renderer());
			Render();
			//Update the screen
			SDL_RenderPresent(mMainWindow->Renderer());
		}
		Destroy();
	}

	//----------------------------------------------------------------------
	bool cSDLApp::HandleEvents()
	{
		bool quit = false;
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			//If user closes the window
			if (e.type == SDL_QUIT)
				quit = true;
			//If user presses any key
			if (e.type == SDL_KEYDOWN)
				quit = true;
			//If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true;
		}
		return !quit;
	}

	//----------------------------------------------------------------------
	
	template<>
	cTextureLib * cSDLApp::Resources<cTextureLib>(const std::string& name) 
	{ 
		return mMainWindow->TextureLib(); 
	}
	
}