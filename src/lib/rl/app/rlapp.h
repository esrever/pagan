#pragma once

#include <glm/glm.hpp>

#include <core/container/box2.h>
#include <core/app/sdlapp.h>
#include <ecs/ecs.h>
#include <rl/dungen/dungen.h>


namespace pgn
{
	namespace rl
	{
		struct cRlApp : public pgn::cSDLApp
		{
			cRlApp(int argc, char ** argv) : pgn::cSDLApp(argc, argv)
			{

			}

			//------------------------------------------------
			virtual void Init();

			//------------------------------------------------
			virtual void Render();
			//------------------------------------------------
			virtual bool Update() 
			{
				return true;
			}
			//------------------------------------------------
			virtual void Destroy(){}

			void LoadBehaviorDb(const char * fname);

			//------------------------------------------------
			size_t	   mNumLines;

			size_t	   mTileDim;
			glm::ivec2 mGridDims;

			ecs::cECS   mECS;

			static const size_t	msTextHeight = 24;
		};
	}
}