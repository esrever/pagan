#include "sdlapp.h"

#include <stdexcept>

#include <SDL.h>
#undef main
#include <SDL_image.h>
//#include <SDL_ttf.h>

namespace pgn
{
	cSDLApp::cSDLApp(int argc, char ** argv)
	{
		//initialize all SDL subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			throw std::runtime_error("SDL Init Failed");
		//if (TTF_Init() == -1)
		//	throw std::runtime_error("TTF Init Failed");
	}

	cSDLApp::~cSDLApp()
	{
		SDL_Quit();
	}
}