#include "sdlapp.h"

#include <stdexcept>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <core/texture/texturelib.h>
#include <core/event/Event.h>

#include "sdlwin.h"

namespace pgn
{
	static cMain g_app = cMain();
	cSDLApp *& mainapp() { return g_app.mApp; }
	ecs::cECS *& mainecs() { return g_app.mECS; }

	//----------------------------------------------------------------------
	cSDLApp::cSDLApp(int argc, char ** argv)
	{
		//initialize all SDL subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			throw std::runtime_error("SDL Init Failed");
		if (TTF_Init() == -1)
			throw std::runtime_error("TTF Init Failed");

		mMainWindow = std::shared_ptr<cSDLWindow>(new cSDLWindow());

		// TODO: fix this! I want ot provide only one arg, or ignore the other two!
		mGameLog.SetFormat(boost::format("%s"));
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
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				evt::cKeyboard::Sig().emit(event.key);
				// TODO: hacky check
				if (event.key.keysym.sym == 27)
					quit = true;
				break;
			case SDL_QUIT:
				evt::cApplicationExit::Sig().emit();
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				evt::cWindow::Sig().emit(event.window);
				break;
			case SDL_MOUSEWHEEL:
				evt::cMouseWheel::Sig().emit(event.wheel);
				break;
			case SDL_MOUSEMOTION:
				evt::cMouseMotion::Sig().emit(event.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				evt::cMouseButton::Sig().emit(event.button);
				break;
			case SDL_FINGERMOTION:
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
				break;
			}
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