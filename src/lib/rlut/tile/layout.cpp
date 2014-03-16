#include "layout.h"

#include <rlut/dungen/dungen.h>

#include <rlut/components/components.h>

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
		//-------------------------------------------------------------------------------
		void cLayout::Init(const rlut::cWorkspace& ws, const std::map<std::string, std::string>& tiles)
		{
			auto& ecs = mainecs();
			auto it_floor = ecs->Archetypes().find(tiles.find("Floor")->second);
			auto it_wall = ecs->Archetypes().find(tiles.find("Wall")->second);
			auto it_door = ecs->Archetypes().find(tiles.find("Door")->second);
			auto it_enter = ecs->Archetypes().find(tiles.find("Entry")->second);
			auto it_exit = ecs->Archetypes().find(tiles.find("Exit")->second);


			// default to wall
			mBgEntities.Resize(ws.mMapData.Width(), ws.mMapData.Height(), it_wall);
			mFgEntities.Resize(ws.mMapData.Width(), ws.mMapData.Height(), ecs->EntitiesToData().end());
			mActors.Resize(ws.mMapData.Width(), ws.mMapData.Height(), ecs->EntitiesToData().end());
			cECS::cEntityWithData it_entry_inst;
			glm::ivec2 entry_coords2;
			for (size_t i = 0; i < ws.mMapData.Height();++i)
			for (size_t j = 0; j < ws.mMapData.Width(); ++j)
			{
				// Instantiate archetypes!
				auto& v = ws.mMapData(j, i);
				if (v & (rlut::eMapData::corridor | rlut::eMapData::room | rlut::eMapData::conn))
					mBgEntities(j, i) = it_floor;
				if (v & rlut::eMapData::conn)
					mFgEntities(j, i) = ecs->Create( it_door->second );
				if (v & rlut::eMapData::entry)
				{
					it_entry_inst = ecs->Create(it_enter->second);
					mFgEntities(j, i) = it_entry_inst;
					entry_coords2 = glm::ivec2(j, i); // alternate, both kinda hacky though.
				}
				if (v & rlut::eMapData::exit)
					mFgEntities(j, i) = ecs->Create(it_exit->second);
			}

			//---- HERO STUFF

			// find entry coordinates, or use entry_coords2
			glm::ivec2 entry_coords(-1, -1);
			const auto& fg_storage = mFgEntities.View().Storage();
			for (const auto& kv : fg_storage.Raw())
				if (kv.second == it_entry_inst)
					entry_coords = glm::ivec2(kv.first % mFgEntities.Width(), kv.first / mFgEntities.Width());
			assert((entry_coords.x >= 0) && (entry_coords.y >= 0));

			// assign loc to hero
			auto it_hero = ecs->Archetypes().find("Hero");
			auto new_hero = ecs->Create(it_hero->second);
			
			auto hero_loc = it_hero->second.Component<cmp::cLocation>();
			
			// Create the component if it doesnt exist. TODO: do I need helper, to do this en masse?
			if (!hero_loc)
			{
				new_hero->second.AddComponent(cComponent<cmp::cLocation>::Create());
				hero_loc = new_hero->second.Component<cmp::cLocation>();
			}
			// TODO: hero needs to get the level ID from the Level, not the layout. So the whole thing here is temporary
			hero_loc->mPos = entry_coords2;
			AddActor(new_hero);

			// TODO: turn system needs to activate hero's keyboard
			new_hero->second.Component<cmp::cActionMap>()->mActionMap.SetActive(true);
		}

		//-------------------------------------------------------------------------------
		void cLayout::AddActor(cECS::cEntityWithDataConst ed)
		{
			auto loc = ed->second.Component<cmp::cLocation>();
			mActors(loc->mPos) = ed;
		}

		//-------------------------------------------------------------------------------
		void cLayout::RemoveActor(cECS::cEntityWithDataConst ed)
		{
			auto loc = ed->second.Component<cmp::cLocation>();
			mActors(loc->mPos) = mActors.View().Storage().GetDefault();
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
					//pmax[i] -= pmin[i];
					pmin[i] = 0;
				}
				
				if (pmax[i] > orgDims[i])
				{
					int vd = pmax[i] - orgDims[i];
					pmin[i] -= vd;
					//pmax[i] -= vd;
				}
			}
			return pmin;

		}
	}
}