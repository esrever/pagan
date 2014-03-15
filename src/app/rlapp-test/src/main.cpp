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
	}

	//------------------------------------------------
	virtual void Render()
	{		
		// get hero and level entities
		auto hero = pgn::mainecs()->TagusToEntities("Hero");
		auto lvl = pgn::mainecs()->TagusToEntities("CurrentLevel");

		// get hero loc and level data
		const auto& hero_pos = hero->second->second.Component<pgn::rl::cmp::cLocation>()->mPos;
		const auto& lvl_layout = lvl->second->second.Component<pgn::rl::cmp::cLevelData>()->mLayout;

		// get the renderrect
		auto view_size = glm::ivec2(mGridDims.x, mGridDims.y);
		auto view_start = pgn::rl::GetCenteredView(glm::ivec2(lvl_layout.BgEntities().Width(), lvl_layout.BgEntities().Height()),
							hero_pos,
							view_size);

		const auto& lvl_bg = lvl_layout.BgEntities();
		const auto& lvl_fg = lvl_layout.FgEntities();
		const auto& lvl_act = lvl_layout.Actors();

		auto tex_atlas = MainWindow()->TextureLib()->FindByName();
		auto tex = tex_atlas->first;
		auto atlas = std::dynamic_pointer_cast<pgn::cTextureAtlas>(tex_atlas->second);

		auto render_tex_func = [](size_t x, size_t y, pgn::cECS::cEntityWithData) {};

		auto bg_view = lvl_bg.CreateView(view_start.x, view_start.y, view_size.x, view_size.y);
		bg_view.VisitRext(render_tex_func);

		auto fg_view = lvl_fg.CreateView(view_start.x, view_start.y, view_size.x, view_size.y);
		fg_view.VisitRext(render_tex_func);

		auto actor_view = lvl_act.CreateView(view_start.x, view_start.y, view_size.x, view_size.y);
		actor_view.VisitRext(render_tex_func);

		/*
		const auto& bg_tex_set = lvl_bg(j, i)->second.Component<pgn::rl::cmp::cTextureSet>();
		pgn::cSubTexture tex = bg_tex_set->mSprites[bg_tex_set->mIndex];
		int v = 255;
		SDL_Rect rect = { j * mTileDim, i * mTileDim, mTileDim, mTileDim };
		MainWindow()->RenderEx(tex.first->Texture(), { v, v, v, 255 }, &tex.second, &rect);
		*/

		pgn::cSDLFont font(MainWindow()->Renderer(), "C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\data\\fonts\\SourceSansPro\\SourceSansPro-Regular.otf", 32);
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
	pgn::mainapp() = new pgn::rlut::cRlApp(argc, argv);
	pgn::mainecs() = new pgn::cECS();
	pgn::mainapp()->Init();

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

	pgn::mainapp()->Run();

	pgn::mainapp()->Destroy();
	delete pgn::mainapp();
	delete pgn::mainecs();
}