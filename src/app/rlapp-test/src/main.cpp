#include <iostream>
#include <algorithm>

#include <pystring.h>
#include <SDL_image.h>

#include <core/app/sdlapp.h>
#include <core/app/sdlwin.h>
#include <core/event/Event.h>
#include <core/math/norm.h>
#include <core/texture/texturelib.h>

#include <glm/glm.hpp>

#include <rl/dungen/dungen.h>
#include <rl/path/difi.h>
#include <rl/fov/FovLookup.h>
#include <rl/fov/fov_rsc.h>

#include <core/serialize/serialize.h>
#include <core/texture/texture.h>
#include <core/texture/texturelib.h>
#include <core/texture/subtexturelib.h>

#include <rl/app/rlapp.h>
#include <rl/components/components.h>
#include <rl/event/events.h>
#include <ecs/ecs.h>
#include <ecs/events.h>
#include <ecs/EntityData.h>
#include <core/serialize/util.h>

#include <core/util/string.h>

#include <rl/systems/Teleport.h>
#include <rl/systems/GameTurn.h>
#include <rl/systems/CreateLevel.h>
#include <rl/systems/RenderMainWin.h>
#include <rl/systems/StatsProc.h>

struct cTestApp : public pgn::rl::cRlApp
{
	cTestApp(int argc, char ** argv) : pgn::rl::cRlApp(argc, argv),
	INIT_EVT_MEMBER(cTestApp, MouseMotion),
	INIT_EVT_MEMBER(cTestApp, Keyboard),
	mMouseOverCell(0,0)
	{
	}

	//------------------------------------------------
	virtual void Init()
	{
		pgn::rl::cRlApp::Init();

		const char * fname_in = PROJECT_ROOT "data\\ecs.xml";
		const char * fname_out = PROJECT_ROOT "data\\ecs_out.xml";

		auto& ecs = *pgn::mainecs();

		auto& gameturn_sys = ecs.System<pgn::ecs::sys::cGameTurn>();

		pugi::xml_document doc_in2;
		if (pgn::LoadXML(doc_in2, fname_in, std::cout))
		{
			doc_in2.print(std::cout);
			pgn::SerializeIn(doc_in2, ecs);

			pugi::xml_document doc_out;
			pgn::SerializeOut(doc_out, "ECS", ecs);
			doc_out.save_file(fname_out);
		}

		// Initialize level
		ecs.System<pgn::ecs::sys::cCreateLevel>()(ecs.Archetypes("TestLevel"));

		// ---- HERO STUFF 
		auto hero = pgn::mainecs()->TagusToEntities("Player");
		
		// set hero as the current player
		pgn::mainecs()->System<pgn::ecs::sys::cGameTurn>().SetCurrent(hero->second);


		// init hero's difi map
		auto& hero_difi = hero->second->second.Component<pgn::ecs::cmp::cMapDiFi>()->mValue;
		// TODO: read the radius from config!
		hero_difi.Init(glm::ivec2(11, 11)); 

		// Create the component -- overwrite it if necessary
		auto hero_loc = ecs.InstantiateComponent<pgn::ecs::cmp::cLocation>(hero->second);

		pgn::mainecs()->System<pgn::ecs::sys::cStatsProc>().InitCreature(hero->second);

		// make hero appear in first level in world
		auto world = pgn::mainecs()->TagusToEntities("World")->second->second.Component<pgn::ecs::cmp::cWorldData>();
		auto lvl = world->mLevelMap.begin()->second->second.Component<pgn::ecs::cmp::cLevelData>();
		pgn::ecs::cmp::cLocation newloc(lvl->mLayout.Entry(), world->mLevelMap.begin()->second->first);
		ecs.System<pgn::ecs::sys::cTeleport>()(hero->second, newloc);

		// ---- MONSTER STUFF 

		// Find free positions
		std::vector<glm::ivec2> free_pos;
		lvl->mLayout.Obstacles().View().VisitRext([&](size_t x, size_t y, bool v){ 
			glm::ivec2 p(x, y);
			if ((!v) && (p != lvl->mLayout.Entry()))
				free_pos.push_back(p);
		;});
		std::random_shuffle(free_pos.begin(), free_pos.end());

		// Get archetype
		auto rat_arch = pgn::mainecs()->Archetypes("Rat");
		int ratNum = std::min(100, int(free_pos.size()));
		int ratCreated = 0;
		pgn::ecs::cmp::cLocation ratloc(glm::ivec2(0,0), world->mLevelMap.begin()->second->first);
		// start adding rats
		while (ratCreated < ratNum)
		{
			ratloc.mPos = free_pos[ratCreated];
			// Instantiate archetype
			auto ed = pgn::mainecs()->InstantiateArchetype(rat_arch->second);
			// InstantiateComponent ControllerAI and cLocation
			ecs.InstantiateComponent<pgn::ecs::cmp::cControllerAI>(ed);
			ecs.InstantiateComponent<pgn::ecs::cmp::cLocation>(ed);
			ecs.System<pgn::ecs::sys::cTeleport>()(ed, ratloc);
			ecs.System<pgn::ecs::sys::cStatsProc>().InitCreature(ed);
			++ratCreated;
		}
	}

	//------------------------------------------------
	virtual void Render()
	{	
		pgn::mainecs()->System<pgn::ecs::sys::cRenderMainWin>().operator()();
		return;
	}
	//------------------------------------------------
	virtual bool Update()
	{
		pgn::mainecs()->System<pgn::ecs::sys::cGameTurn>()();
		return true;
	}
	//------------------------------------------------
	virtual void Destroy(){}

	glm::ivec2 mMouseOverCell;

	//DECL_EVT_MEMBER(MouseMotion);
	void OnMouseMotion(const SDL_MouseMotionEvent& e)
	{
		mMouseOverCell.x = std::min( int(e.x / mTileDim), mGridDims.x-1);
		mMouseOverCell.y = std::min(int(e.y / mTileDim), mGridDims.y - 1);
		//mMouseOverCell.y = mGridDims.y - 1 - mMouseOverCell.y;
	}

	void OnKeyboard(const SDL_KeyboardEvent& e)
	{
		std::cout << e.keysym.scancode << " - " << e.keysym.sym << std::endl;
		if (e.state == 1)
		{
			if (e.keysym.scancode == SDL_SCANCODE_KP_PLUS)
			{
			}
			else if (e.keysym.scancode == SDL_SCANCODE_KP_MINUS)
			{
			}
		}
	}

	DECL_EVT_MEMBER(MouseMotion);
	DECL_EVT_MEMBER(Keyboard);
	
	static const size_t	msTextHeight = 24;
};

int main(int argc, char ** argv)
{
	std::cout << PROJECT_ROOT << std::endl;
	pgn::mainapp() = new cTestApp(argc, argv);
	pgn::mainecs() = new pgn::ecs::cECS();

	pgn::mainapp()->Run();

	pgn::mainapp()->Destroy();
	delete pgn::mainapp();
	delete pgn::mainecs();
}