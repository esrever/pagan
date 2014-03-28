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
		auto presetnode = node.child("Preset");
		if (presetnode.empty())
		{
			pgn::rl::cWorkspace ws;
			ret += pgn::SerializeIn(node, "DunGen", ws);
			value.Init(ws.mMapData, tiles);
		}
		else
		{
			// TODO: this is a mess!
			std::string fname;
			pgn::SerializeIn(presetnode.child("File"), fname);
			fname = PROJECT_ROOT + fname;

			std::map<std::string, glm::ivec4> legend;
			pgn::SerializeIn(presetnode.child("Legend"), legend);

			auto surface = IMG_Load(fname.c_str());
			cArray2D<int> dmap(surface->w, surface->h,0);
			unsigned char * pixels = (unsigned char *)(surface->pixels);
			for (const auto& kv : legend)
			{
				for (int i = 0; i < surface->h; ++i)
					for (int j = 0; j < surface->w; ++j)
					{
						auto o = (surface->h-1-i)*surface->pitch + 4 * j;
						glm::ivec4 c(pixels[o], pixels[o + 1], pixels[o + 2], pixels[o + 3]);
						if (kv.second == c)
						{
							if (kv.first == "Floor")
								dmap(j, i) = pgn::rl::eMapData::room;
							else if (kv.first == "Wall")
								dmap(j, i) = pgn::rl::eMapData::perimeter;
							else if (kv.first == "Door")
								dmap(j, i) = pgn::rl::eMapData::conn | pgn::rl::eMapData::room;
							else if (kv.first == "Entry")
								dmap(j, i) = pgn::rl::eMapData::entry | pgn::rl::eMapData::room;
							else if (kv.first == "Exit")
								dmap(j, i) = pgn::rl::eMapData::exit | pgn::rl::eMapData::room;
						}
					}
			}

			value.Init(dmap, tiles);
		}

		return ret;
	}

	namespace rl
	{
		//-------------------------------------------------------------------------------
		void cLayout::Init(const cArray2D<int>& dmap, const std::map<std::string, std::string>& tiles)
		{
			auto& ecs = mainecs();
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