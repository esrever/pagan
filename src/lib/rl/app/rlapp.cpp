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
#include <rl/systems/RenderMainWin.h>
#include <rl/systems/RenderStatus.h>
#include <rl/systems/RenderLog.h>
#include <rl/systems/UpdateAI.h>
#include <rl/systems/UpdateDiFi.h>


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

			mGridDims= glm::ivec2(40, 20);
			mTileDim = 32;

			// TODO: move system init elsewhere
			pgn::mainecs()->System<ecs::sys::cInputKey>();
			pgn::mainecs()->System<ecs::sys::cCalcVis>();
			pgn::mainecs()->System<ecs::sys::cCreateLevel>();
			pgn::mainecs()->System<pgn::ecs::sys::cGameTurn>();
			pgn::mainecs()->System<pgn::ecs::sys::cCreateLevel>();
			pgn::mainecs()->System<pgn::ecs::sys::cUpdateLayout>();
			pgn::mainecs()->System<pgn::ecs::sys::cUpdateAI>();
			pgn::mainecs()->System<pgn::ecs::sys::cUpdateDiFi>();

			pgn::mainecs()->System<pgn::ecs::sys::cRenderMainWin>().SetWindow(MainWindow().get());
			pgn::mainecs()->System<pgn::ecs::sys::cRenderMainWin>().SetLayout(mGridDims, mTileDim);


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