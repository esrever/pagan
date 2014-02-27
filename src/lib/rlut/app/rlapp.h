#pragma once

#include <glm/glm.hpp>

#include <core/app/sdlapp.h>
#include <rlut/sprite/atlas.h>
#include <rlut/dungen/dungen.h>

namespace pgn
{
	namespace rlut
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

			//------------------------------------------------
			glm::uvec2 mGridDims;
			size_t	   mNumLines;
			pgn::rlut::cSpriteAtlas mSpriteAtlas;

			pgn::rlut::cWorkspace mDungeon;

			size_t	   mTileDim;
			glm::uvec2 mGridStart;
			glm::uvec2 mLogStart;
			glm::uvec2 mStatusStart;

			static const size_t	msTextHeight = 20;
		};
	}
}