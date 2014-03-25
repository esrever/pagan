#include "rlapp.h"

#include <algorithm>
#include <core/app/sdlwin.h>
#include <core/sdl/font.h>

#include <core/texture/texturelib.h>
#include <core/serialize/util.h>

#include <rl/ai/actions.h>
#include <rl/ai/behavior.h>
#include <rl/ai/conditions.h>
#include <rl/components/RegisterComponents.h>
#include <rl/event/events.h>
#include <rl/event/actions.h>

#include <ecs/systems/InputKey.h>
#include <rl/systems/CalcVis.h>
#include <rl/systems/CreateLevel.h>
#include <rl/systems/GameTurn.h>
#include <rl/systems/CreateLevel.h>
#include <rl/systems/UpdateLayout.h>
#include <rl/systems/RenderGameMap.h>
#include <rl/systems/UpdateAI.h>

namespace pgn
{
	namespace rl
	{
		void cRlApp::Init()
		{
			// TODO: somewhere else? configurable?
			srand(0);

			// Initialize AI-related
			pgn::bt::RegisterActions();
			pgn::bt::RegisterConditions();
			pgn::ecs::RegisterComponents(*mainecs());
			pgn::rl::RegisterActions(*mainecs());

			static const glm::uvec2 gridDims(40, 20);
			static const size_t  numLines = 4;

			mGridArea.mDims = gridDims;
			mNumLines = numLines;
			glm::ivec2 windims(MainWindow()->Rect().w, MainWindow()->Rect().h);

			const size_t logsize = mNumLines * msTextHeight + msTextHeight / 4;
			int tile_h = (windims.y - logsize) / mGridArea.mDims.y;
			int tile_w = windims.x / mGridArea.mDims.x;
			mTileDim = std::min(tile_w, tile_h);

			mGridArea.mStart= glm::uvec2(0, 0);
			mLogStart = glm::uvec2(5, mTileDim*mGridArea.mDims.y + msTextHeight / 4);
			mStatusStart = glm::uvec2(5 + mTileDim*mGridArea.mDims.x, 0);

			// TODO: move system init elsewhere
			pgn::mainecs()->System<ecs::sys::cInputKey>();
			pgn::mainecs()->System<ecs::sys::cCalcVis>();
			pgn::mainecs()->System<ecs::sys::cCreateLevel>();
			pgn::mainecs()->System<pgn::ecs::sys::cGameTurn>();
			pgn::mainecs()->System<pgn::ecs::sys::cCreateLevel>();
			pgn::mainecs()->System<pgn::ecs::sys::cUpdateLayout>();
			pgn::mainecs()->System<pgn::ecs::sys::cUpdateAI>();
			pgn::mainecs()->System<pgn::ecs::sys::cRenderGameMap>().SetArea( mGridArea, mTileDim);
			pgn::mainecs()->System<pgn::ecs::sys::cRenderGameMap>().SetWindow(MainWindow().get());

			const char * fname_atlas = PROJECT_ROOT "data\\tiledesc.xml";
			MainWindow()->TextureLib()->Load(fname_atlas, "");

			const char * fname_bt = PROJECT_ROOT "data\\behaviors.xml";
			pgn::mainecs()->System<pgn::ecs::sys::cUpdateAI>().LoadBehaviorDb(fname_bt);
		}

		void cRlApp::Render()
		{
			
		}
	}
}