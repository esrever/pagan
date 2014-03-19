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

#include <rlut/dungen/dungen.h>
#include <rlut/path/difi.h>
#include <rlut/fov/FovLookup.h>
#include <rlut/fov/fov_rsc.h>

#include <core/serialize/serialize.h>
#include <core/texture/texture.h>
#include <core/texture/texturelib.h>
#include <core/texture/subtexturelib.h>

#include <rlut/app/rlapp.h>
#include <rlut/components/components.h>
#include <rlut/event/events.h>
#include <ecs/ecs.h>
#include <ecs/events.h>
#include <ecs/EntityData.h>
#include <core/serialize/util.h>

#include <core/util/string.h>

struct cTestApp : public pgn::rlut::cRlApp
{
	cTestApp(int argc, char ** argv) : pgn::rlut::cRlApp(argc, argv),
	INIT_EVT_MEMBER(cTestApp, MouseMotion),
	INIT_EVT_MEMBER(cTestApp, Keyboard),
	mMouseOverCell(0,0)
	{
	}

	//------------------------------------------------
	virtual void Init()
	{
		pgn::rlut::cRlApp::Init();

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
		//pgn::evt::cCreateLevel::Run(lvl0);
		// TODO: 
		//ecs.System<pgn::sys::cCreateLevel>()->Execute(lvl0);

		// ---- HERO STUFF 
		auto hero = pgn::mainecs()->TagusToEntities("Player");
		auto hero_loc = hero->second->second.Component<pgn::ecs::cmp::cLocation>();
		// Create the component if it doesnt exist. 
		if (!hero_loc)
		{
			hero->second->second.AddComponent(pgn::ecs::cComponent<pgn::ecs::cmp::cLocation>::Create());
			hero_loc = hero->second->second.Component<pgn::ecs::cmp::cLocation>();
		}

		// make hero appear in first level in world
		auto world = pgn::mainecs()->TagusToEntities("World")->second->second.Component<pgn::ecs::cmp::cWorldData>();
		auto lvl = world->mLevelMap.begin()->second->second.Component<pgn::ecs::cmp::cLevelData>();
		hero_loc->mPos = lvl->mLayout.Entry();
		hero_loc->mLevelId = world->mLevelMap.begin()->second->first;
		auto& hero_pos = hero_loc->mPos;
		// pgn::evt::cPlayerAppear::Run(*hero_loc);
		// TODO: emit LocationChanged!
	}

	//------------------------------------------------
	virtual void Render()
	{		
		return;
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
		auto view_start = pgn::rl::GetCenteredView(glm::ivec2(lvl_layout.BgEntities().Width(), lvl_layout.BgEntities().Height()),
							hero_pos,
							view_size);
		view_size.x = std::min(view_size.x, int(lvl_layout.BgEntities().Width() - view_start.x));
		view_size.y = std::min(view_size.y, int(lvl_layout.BgEntities().Height() - view_start.y));
		auto view_end = view_start + view_size;

		auto& lvl_bg = lvl_layout.BgEntities();
		auto& lvl_fg = lvl_layout.FgEntities();
		auto& lvl_act = lvl_layout.Actors();

		auto tex_atlas = MainWindow()->TextureLib()->FindByName();
		auto tex = tex_atlas->first;
		auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);

		// TODO: get appr. level id
		auto& visview = hero_vis->mVisible[lvl_id].CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
		auto& expview = hero_vis->mExplored[lvl_id].CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
		std::function<int(int x, int y)> get_fow = [&](int x, int y){return visview(x, y) ? 255 : (expview(x, y) ? 100 : 0); };

		auto render_func_sparse = [&](const pgn::rl::cLayout::sparse_entities_type& map)
		{
			if (map.View().Storage().Raw().empty())
				return;
			for (const auto& kv : map.View().Storage().Raw())
			{
				auto ed = kv.second->second;
				auto idx1 = kv.first;
				glm::ivec2 idx(idx1 % map.Width(), idx1 / map.Width());

				if ((idx.x < view_start.x) || (idx.y < view_start.y) || (idx.x >= view_end.x) || (idx.y >= view_end.y))
					continue;
					
				auto x = idx.x - view_start.x;
				auto y = idx.y - view_start.y;
				const auto& bg_tex_set = ed.Component<pgn::ecs::cmp::cTextureSet>();
				pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
				int v = get_fow(x,y);
				SDL_Rect rect = { x * mTileDim, (mGridDims.y-1-y) * mTileDim, mTileDim, mTileDim };
				MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
			}
		};

		auto render_impl_func = [&](size_t x, size_t y, const pgn::ecs::cEntityData& ed)
		{
			const auto& bg_tex_set = ed.Component<pgn::ecs::cmp::cTextureSet>();
			pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
			int v = get_fow(x, y);
			SDL_Rect rect = { x * mTileDim, (mGridDims.y - 1 - y) * mTileDim, mTileDim, mTileDim };
			MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
		};

		auto render_func_dense = [&](size_t x, size_t y, const pgn::ecs::cArchetypeWithDataConst& ed) { render_impl_func(x, y, ed->second); };

		auto bg_view = lvl_bg.CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
		bg_view.VisitRext(render_func_dense);

		// TODO: sparse array views?
		render_func_sparse(lvl_fg);

		if (!lvl_act.empty())
		{
			for (const auto& kv : lvl_act)
			{
				auto x = kv.second.x - view_start.x;
				auto y = kv.second.y - view_start.y;
				const auto& bg_tex_set = kv.first->second.Component<pgn::ecs::cmp::cTextureSet>();
				pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
				int v = get_fow(x, y);
				SDL_Rect rect = { x * mTileDim, (mGridDims.y - 1 - y) * mTileDim, mTileDim, mTileDim };
				MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
			}
		}

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