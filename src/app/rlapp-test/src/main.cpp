#include <iostream>
#include <algorithm>

#include <pystring.h>
#include <SDL_image.h>

#include <core/app/sdlapp.h>
#include <core/app/sdlwin.h>
#include <core/event/Event.h>
#include <core/sdl/font.h>
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

		pugi::xml_document doc_in2;
		if (pgn::LoadXML(doc_in2, fname_in, std::cout))
		{
			doc_in2.print(std::cout);
			pgn::SerializeIn(doc_in2, ecs);

			pugi::xml_document doc_out;
			pgn::SerializeOut(doc_out, "ECS", ecs);
			doc_out.save_file(fname_out);
		}

		auto lvl0 = *pgn::mainecs()->TagsToEntities("TestLevel")->second.begin();
		pgn::evt::cLevelCreated::Sig().emit(lvl0);

		// ---- HERO STUFF 
		auto hero = pgn::mainecs()->TagusToEntities("Player");
		
		// set hero as the current player
		pgn::mainecs()->System<pgn::ecs::sys::cGameTurn>().SetCurrent(hero->second);

		// Create the component -- overwrite it if necessary
		auto hero_loc = ecs.InstantiateComponent<pgn::ecs::cmp::cLocation>(hero->second);

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
			++ratCreated;
		}
	}

	//------------------------------------------------
	virtual void Render()
	{		
		auto& ecs = pgn::mainecs();
		// get hero and level entities
		auto hero = pgn::mainecs()->TagusToEntities("Player");
		auto lvl = pgn::mainecs()->TagusToEntities("CurrentLevel");

		// get hero loc and level data
		auto& hero_pos = hero->second->second.Component<pgn::ecs::cmp::cLocation>()->mPos;
		auto hero_vis = hero->second->second.Component<pgn::ecs::cmp::cVisibility>();
		auto& lvl_layout = lvl->second->second.Component<pgn::ecs::cmp::cLevelData>()->mLayout;
		auto lvl_id = lvl->second->first;

		// get the renderrect
		auto view_size = glm::ivec2(mGridDims.x, mGridDims.y);
		auto view_start = pgn::rl::GetCenteredView(glm::ivec2(lvl_layout.Dims().x, lvl_layout.Dims().y),
							hero_pos,
							view_size);
		view_size.x = std::min(view_size.x, int(lvl_layout.Dims().x - view_start.x));
		view_size.y = std::min(view_size.y, int(lvl_layout.Dims().y - view_start.y));
		auto view_end = view_start + view_size;

		auto& lvl_bg = lvl_layout.Bg().Cells();
		auto& lvl_fg = lvl_layout.Fg().Cells();
		auto& lvl_act = lvl_layout.Actors();

		auto tex_atlas = MainWindow()->TextureLib()->FindByName();
		auto tex = tex_atlas->first;
		auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);

		auto& visview = hero_vis->mVisible[lvl_id].CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
		auto& expview = hero_vis->mExplored[lvl_id].CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
		std::function<int(int x, int y)> get_fow = [&](int x, int y){return visview(x, y) ? 255 : (expview(x, y) ? 100 : 0); };

		// TODO: this is crap. rethink views
		auto render_func_dense = [&](const pgn::cArray2D<pgn::ecs::cEntityWithDataConst>& cells){
			for (int i = view_start.y; i < (view_start.y + view_size.y); ++i)
			for (int j = view_start.x; j < (view_start.x + view_size.x); ++j)
			{
				auto x = j - view_start.x;
				auto y = i - view_start.y;

				auto& ed = cells(j, i);
				const auto& bg_tex_set = ed->second.Component<pgn::ecs::cmp::cTextureSet>();
				pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
				int v = get_fow(x, y);
				SDL_Rect rect = { x * mTileDim, (mGridDims.y - 1 - y) * mTileDim, mTileDim, mTileDim };
				MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
			}
		};

		auto render_func_sparse = [&](const std::set<pgn::ecs::cEntityWithDataConst>& ents)
		{
			if (ents.empty())
				return;
			for (const auto& ed : ents)
			{
				const auto& pos = ed->second.Component<pgn::ecs::cmp::cLocation>()->mPos;

				if ((pos.x < view_start.x) || (pos.y < view_start.y) || (pos.x >= view_end.x) || (pos.y >= view_end.y))
					continue;
					
				auto x = pos.x - view_start.x;
				auto y = pos.y - view_start.y;
				const auto& bg_tex_set = ed->second.Component<pgn::ecs::cmp::cTextureSet>();
				pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
				int v = get_fow(x,y);
				SDL_Rect rect = { x * mTileDim, (mGridDims.y-1-y) * mTileDim, mTileDim, mTileDim };
				MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
			}
		};

		render_func_dense(lvl_layout.Bg().Cells());
		render_func_sparse(lvl_layout.Fg().Entities());
		render_func_sparse(lvl_layout.Actors().Entities());

		pgn::cSDLFont font(MainWindow()->Renderer(), PROJECT_ROOT "data\\fonts\\SourceSansPro\\SourceSansPro-Regular.otf", 32);
		//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\PT-Sans\\PTN57F.ttf", 62);
		//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\Nobile\\Nobile-Regular.ttf", 32);

		SDL_Rect textRect;
		auto& gamelog = pgn::mainapp()->GameLog();
		size_t i = 0;
		for (const auto& line : gamelog.Data())
		{
			auto texf = font.CreateText(line, &textRect);
			float ar = textRect.w / float(textRect.h);

			size_t yo = mLogStart.y + i*msTextHeight;
			SDL_Rect rect = { mLogStart.x, yo, int(ar * msTextHeight), msTextHeight };
			MainWindow()->Render(texf.get(), &rect);
			++i;
			if (i == mNumLines)
				break;
		}
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
		mMouseOverCell.x = std::min( e.x / mTileDim, mGridDims.x-1);
		mMouseOverCell.y = std::min(e.y / mTileDim, mGridDims.y - 1);
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