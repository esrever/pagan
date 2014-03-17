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
#include <ecs/ecs.h>
#include <ecs/EntityData.h>
#include <core/serialize/util.h>

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

		pugi::xml_document doc_in2;
		if (pgn::LoadXML(doc_in2, fname_in, std::cout))
		{
			doc_in2.print(std::cout);
			SerializeIn(doc_in2, pgn::ECS());

			pugi::xml_document doc_out;
			SerializeOut(doc_out, "ECS", pgn::ECS());
			doc_out.save_file(fname_out);
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
		auto& hero_pos = hero->second->second.Component<pgn::rl::cmp::cLocation>()->mPos;
		auto& lvl_layout = lvl->second->second.Component<pgn::rl::cmp::cLevelData>()->mLayout;

		// get the renderrect
		auto view_size = glm::ivec2(mGridDims.x, mGridDims.y);
		auto view_start = pgn::rl::GetCenteredView(glm::ivec2(lvl_layout.BgEntities().Width(), lvl_layout.BgEntities().Height()),
							hero_pos,
							view_size);
		auto view_end = view_start + view_size;

		auto& lvl_bg = lvl_layout.BgEntities();
		auto& lvl_fg = lvl_layout.FgEntities();
		auto& lvl_act = lvl_layout.Actors();

		auto tex_atlas = MainWindow()->TextureLib()->FindByName();
		auto tex = tex_atlas->first;
		auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);

		auto render_func_sparse = [&](const pgn::rl::cLayout::sparse_entities_type& map)
		{
			for (const auto& kv : map.View().Storage().Raw())
			{
				auto ed = kv.second->second;
				auto idx1 = kv.first;
				glm::ivec2 idx(idx1 % map.Width(), idx1 / map.Width());

				if ((idx.x < view_start.x) || (idx.y < view_start.y) || (idx.x >= view_end.x) || (idx.y >= view_end.y))
					continue;
					
				auto x = idx.x - view_start.x;
				auto y = idx.y - view_start.y;
				const auto& bg_tex_set = ed.Component<pgn::rl::cmp::cTextureSet>();
				pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
				int v = 255;
				SDL_Rect rect = { x * mTileDim, y * mTileDim, mTileDim, mTileDim };
				MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
			}
		};

		auto render_impl_func = [&](size_t x, size_t y, const pgn::cEntityData& ed)
		{
			const auto& bg_tex_set = ed.Component<pgn::rl::cmp::cTextureSet>();
			pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
			int v = 255;
			SDL_Rect rect = { x * mTileDim, y * mTileDim, mTileDim, mTileDim };
			MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
		};

		auto render_func_dense = [&](size_t x, size_t y, const pgn::cECS::cArchetypeWithDataConst& ed) { render_impl_func(x, y, ed->second); };

		auto bg_view = lvl_bg.CreateView(view_size.x, view_size.y, view_start.x, view_start.y);
		bg_view.VisitRext(render_func_dense);

		// TODO: use views! sparse views
		render_func_sparse(lvl_fg);
		render_func_sparse(lvl_act);

		pgn::cSDLFont font(MainWindow()->Renderer(), PROJECT_ROOT "data\\fonts\\SourceSansPro\\SourceSansPro-Regular.otf", 32);
		//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\PT-Sans\\PTN57F.ttf", 62);
		//pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\Nobile\\Nobile-Regular.ttf", 32);

		SDL_Rect textRect;
		char buf[256];
		sprintf(buf, "%d %d", mMouseOverCell.x, mMouseOverCell.y);
		auto texf = font.CreateText(buf,&textRect);

		float ar = textRect.w / float(textRect.h);

		for (size_t i = 0; i < mNumLines; ++i)
		{
			size_t yo = mLogStart.y + i*msTextHeight;
			SDL_Rect rect = { mLogStart.x, yo, int(ar * msTextHeight), msTextHeight };
			MainWindow()->Render(texf.get(), &rect);
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
	pgn::rl::cActionMap am;
};

int main(int argc, char ** argv)
{
	std::cout << PROJECT_ROOT << std::endl;
	pgn::mainapp() = new cTestApp(argc, argv);
	pgn::mainecs() = new pgn::cECS();

	pgn::mainapp()->Run();

	pgn::mainapp()->Destroy();
	delete pgn::mainapp();
	delete pgn::mainecs();
}