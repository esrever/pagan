#include "layout.h"

#include <rlut/dungen/dungen.h>

#include <rlut/components/AsciiSet.h>

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const rl::cLayout& value)
	{
		std::vector<std::string> ss;
		for (size_t i = 0; i < value.BgEntities().Height(); ++i)
		{
			ss.push_back("");
			std::string& s = ss.back();
			for (size_t j = 0; j < value.BgEntities().Width(); ++j)
			{
				const auto& v = value.BgEntities()(j, i);
				const auto& vf = value.FgEntities()(j, i);
				if (&vf != &value.FgEntities().View().Storage().GetDefault())
				{
					const auto& syms = vf->second.Component<rl::cmp::cAsciiSet>()->mSymbols;
					s.append(syms.empty() ? "?" : syms.front());
				}
				else
				{
					const auto& syms = v->second.Component<rl::cmp::cAsciiSet>()->mSymbols;
					s.append(syms.empty() ? "?" : syms.front());
				}
			}
		}
		SerializeOut(node, "DunGen", ss);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, rl::cLayout& value)
	{
		size_t ret = 0;

		// Read dungeon spec and tiles
		pgn::rlut::cWorkspace ws;
		ret += pgn::SerializeIn(node, "DunGen", ws);

		std::map<std::string, std::string> tiles;
		ret += pgn::SerializeIn(node, "Tiles", tiles);
		
		value.Init(ws, tiles);

		return ret;
	}

	namespace rl
	{
		void cLayout::Init(const rlut::cWorkspace& ws, const std::map<std::string, std::string>& tiles)
		{
			auto& ecs = mainecs();
			auto it_floor = ecs->Archetypes().find(tiles.find("Floor")->second);
			auto it_wall = ecs->Archetypes().find(tiles.find("Wall")->second);
			auto it_door = ecs->Archetypes().find(tiles.find("Door")->second);
			auto it_enter = ecs->Archetypes().find(tiles.find("GateEnter")->second);
			auto it_exit = ecs->Archetypes().find(tiles.find("GateExit")->second);


			// default to wall
			mBgEntities.Resize(ws.mMapData.Width(), ws.mMapData.Height(), it_wall);
			mFgEntities.Resize(ws.mMapData.Width(), ws.mMapData.Height(), ecs->EntitiesToData().end());
			for (size_t i = 0; i < ws.mMapData.Height();++i)
			for (size_t j = 0; j < ws.mMapData.Width(); ++j)
			{
				// Instantiate archetypes!
				auto& v = ws.mMapData(j, i);
				if (v & (rlut::eMapData::corridor | rlut::eMapData::room))
					mBgEntities(j, i) = it_floor;
				if (v & rlut::eMapData::conn)
					mFgEntities(j, i) = ecs->Create( it_door->second );
				if (v & rlut::eMapData::entry)
					mFgEntities(j, i) = ecs->Create(it_enter->second);
				if (v & rlut::eMapData::exit)
					mFgEntities(j, i) = ecs->Create(it_exit->second);
			}
		}
	}
}