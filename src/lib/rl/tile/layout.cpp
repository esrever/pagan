#include "layout.h"

#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_image.h>

#include <rl/dungen/dungen.h>

#include <rl/components/components.h>
#include <rl/event/events.h>

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const rl::cLayout& value)
	{
		std::vector<std::string> ss;
		for (size_t i = 0; i < value.Dims().y; ++i)
		{
			ss.push_back("");
			std::string& s = ss.back();
			for (size_t j = 0; j < value.Dims().x; ++j)
			{
				const auto& v = value.Bg().Cells()(j, i);
				const auto& syms = v->second.Component<ecs::cmp::cAsciiSet>()->mSymbols;
				s.append(syms.empty() ? "?" : syms.front());
			}
		}
		SerializeOut(node, "DunGen", ss);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, rl::cLayout& value)
	{
		size_t ret = 0;

		std::map<std::string, std::string> tiles;
		ret += pgn::SerializeIn(node, "Tiles", tiles);

		// Read dungeon spec and tiles
		cArray2D<SDL_Color> dmap_col;
		auto presetnode = node.child("Preset");
		if (presetnode.empty())
		{
			pgn::rl::cWorkspace ws;
			ret += pgn::SerializeIn(node, "DunGen", ws);
			//value.Init(ws.mMapData, tiles);
			// TODO: read colourmap
			/*
			std::map<int, SDL_Color> cmap;
			ret += pgn::SerializeIn(node.child("ColorMap"), cmap);
			dmap_col.Resize(ws.mMapData.Width(), ws.mMapData.Height());
			*/

		}
		else
		{
			// TODO: this is a mess!
			std::string fname;
			pgn::SerializeIn(presetnode.child("File"), fname);
			fname = PROJECT_ROOT + fname;

			// Parse the legend data
			SDL_Color def_color;
			std::map< SDL_Color, rl::cLayout::legend_entry_t> legend_entries;
			for (const auto& c : presetnode.children())
			{
				std::string arch_str;
				std::string tile_type_str;
				SDL_Color color;
				std::string tag;
				bool is_default = false;

				SerializeIn(c, "color", color);
				SerializeIn(c, "arch", arch_str);
				SerializeIn(c, "tile_type", tile_type_str);
				SerializeIn(c, "default", is_default);
				SerializeIn(c, "tag", tag);


				auto arch = mainecs()->Archetypes(arch_str);

				// Instantiate bg entity here
				if (tile_type_str == "bg")
				{
					auto ent = mainecs()->InstantiateArchetype(arch->second);
					// set the default if necessary
					if (is_default)
						def_color = color;
				}

				legend_entries.emplace( color, std::make_tuple(arch, tile_type_str, is_default, tag));
			}

			// load image
			auto surface = IMG_Load(fname.c_str());
			unsigned char * pixels = (unsigned char *)(surface->pixels);

			// write data to color array
			cArray2D<SDL_Color> cmap(surface->w, surface->h, def_color);
			cmap.View().VisitWext([&](size_t x, size_t y, SDL_Color& c){ 
				auto o = (surface->h - 1 - y)*surface->pitch + 4 * x;
				c.r = pixels[o];
				c.g = pixels[o+1];
				c.b = pixels[o+2];
				c.a = pixels[o+3];
			});
		
			value.Init(cmap, legend_entries);
		}

		return ret;
	}

	namespace rl
	{
		//-------------------------------------------------------------------------------
		void cLayout::Init(const cArray2D<SDL_Color>& cmap, const std::map<SDL_Color, legend_entry_t>& legend)
		{
			auto& ecs = mainecs();

			for (size_t y = 0; y < cmap.Height(); ++y)
			for (size_t x = 0; x < cmap.Width(); ++x)
			{
				// fetch color and then tuple 
				const SDL_Color& c = cmap(x, y);
				auto legit = legend.find(c);
				assert(legit != legend.end());

				// get tuple data
				auto arch = std::get<0>(legit->second);
				const auto& tile_type_str = std::get<1>(legit->second);
				auto is_default = std::get<2>(legit->second);
				const auto& tag = std::get<3>(legit->second);

				if (tile_type_str == "bg")
				{
					// fetch instance & set it to bg store
					mainecs()->
				}
				else
				{
					// create instance
				}
			}



			auto it_floor = mainecs()->InstantiateArchetype( ecs->Archetypes().find(tiles.find("Floor")->second)->second);
			auto it_wall = mainecs()->InstantiateArchetype(ecs->Archetypes().find(tiles.find("Wall")->second)->second);
			auto it_door = ecs->Archetypes().find(tiles.find("Door")->second);
			auto it_enter = ecs->Archetypes().find(tiles.find("Entry")->second);
			auto it_exit = ecs->Archetypes().find(tiles.find("Exit")->second);

			mDims.x = dmap.Width();
			mDims.y = dmap.Height();

			// default to wall
			mBg.Resize(dmap.Width(), dmap.Height(), it_wall);
			mFg.Resize(dmap.Width(), dmap.Height(), it_wall);
			mActors.Resize(dmap.Width(), dmap.Height(), it_wall);
			for (size_t i = 0; i < dmap.Height();++i)
			for (size_t j = 0; j < dmap.Width(); ++j)
			{
				glm::ivec2 pos(j, i);
				// Instantiate archetypes!
				auto& v = dmap(j, i);
				if (v & (rl::eMapData::corridor | rl::eMapData::room | rl::eMapData::conn))
					mBg.Add(it_floor, pos);
				if (v & rl::eMapData::conn)
				{
					auto ed = mainecs()->InstantiateArchetype(it_door->second);
					ed->second.AddComponent<ecs::cmp::cLocation>()->mPos = pos;
					mFg.Add(ed, pos);
				}
				if (v & rl::eMapData::entry)
				{
					auto ed = mainecs()->InstantiateArchetype(it_enter->second);
					ed->second.AddComponent<ecs::cmp::cLocation>()->mPos = pos;
					mFg.Add(ed, pos);
					mEntry = pos;
				}
				if (v & rl::eMapData::exit)
				{
					auto ed = mainecs()->InstantiateArchetype(it_exit->second);
					ed->second.AddComponent<ecs::cmp::cLocation>()->mPos = pos;
					mFg.Add(ed, pos);
					mExit = pos;
				}		
			}
		}

		//-------------------------------------------------------------------------------
		glm::ivec2 GetCenteredView(const glm::ivec2& orgDims,
					  			   const glm::ivec2& center,
								   const glm::ivec2& tgtSize)
		{
			glm::ivec2 pmin = center - (tgtSize / 2);
			glm::ivec2 pmax = pmin + tgtSize;

			for (int i = 0; i < 2;++i)
			{
				if (pmin[i] < 0)
				{
					pmax[i] -= pmin[i];
					pmin[i] = 0;
				}
				
				if (pmax[i] > orgDims[i])
				{
					int vd = pmax[i] - orgDims[i];
					pmin[i] -= vd;
					//pmax[i] -= vd;
					pmin[i] = std::max(pmin[i], 0);
				}
			}
			return pmin;

		}
	}
}