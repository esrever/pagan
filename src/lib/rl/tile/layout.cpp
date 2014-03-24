#include "layout.h"

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

		// Read dungeon spec and tiles
		pgn::rl::cWorkspace ws;
		ret += pgn::SerializeIn(node, "DunGen", ws);

		std::map<std::string, std::string> tiles;
		ret += pgn::SerializeIn(node, "Tiles", tiles);
		
		value.Init(ws, tiles);

		return ret;
	}

	namespace rl
	{
		//-------------------------------------------------------------------------------
		void cLayout::Init(const rl::cWorkspace& ws, const std::map<std::string, std::string>& tiles)
		{
			auto& ecs = mainecs();
			auto it_floor = mainecs()->InstantiateArchetype( ecs->Archetypes().find(tiles.find("Floor")->second)->second);
			auto it_wall = mainecs()->InstantiateArchetype(ecs->Archetypes().find(tiles.find("Wall")->second)->second);
			auto it_door = ecs->Archetypes().find(tiles.find("Door")->second);
			auto it_enter = ecs->Archetypes().find(tiles.find("Entry")->second);
			auto it_exit = ecs->Archetypes().find(tiles.find("Exit")->second);

			mDims.x = ws.mMapData.Width();
			mDims.y = ws.mMapData.Height();

			// default to wall
			mBg.Resize(ws.mMapData.Width(), ws.mMapData.Height(), it_wall);
			mFg.Resize(ws.mMapData.Width(), ws.mMapData.Height(), it_wall);
			mActors.Resize(ws.mMapData.Width(), ws.mMapData.Height(), it_wall);
			for (size_t i = 0; i < ws.mMapData.Height();++i)
			for (size_t j = 0; j < ws.mMapData.Width(); ++j)
			{
				glm::ivec2 pos(j, i);
				// Instantiate archetypes!
				auto& v = ws.mMapData(j, i);
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