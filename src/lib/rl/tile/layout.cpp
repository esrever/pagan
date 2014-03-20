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
					const auto& syms = vf->second.Component<ecs::cmp::cAsciiSet>()->mSymbols;
					s.append(syms.empty() ? "?" : syms.front());
				}
				else
				{
					const auto& syms = v->second.Component<ecs::cmp::cAsciiSet>()->mSymbols;
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
			auto it_floor = ecs->Archetypes().find(tiles.find("Floor")->second);
			auto it_wall = ecs->Archetypes().find(tiles.find("Wall")->second);
			auto it_door = ecs->Archetypes().find(tiles.find("Door")->second);
			auto it_enter = ecs->Archetypes().find(tiles.find("Entry")->second);
			auto it_exit = ecs->Archetypes().find(tiles.find("Exit")->second);


			// default to wall
			mBgEntities.Resize(ws.mMapData.Width(), ws.mMapData.Height(), it_wall);
			mFgEntities.Resize(ws.mMapData.Width(), ws.mMapData.Height(), ecs->EntitiesToData().end());
			mActors.clear();
			ecs::cEntityWithData it_entry_inst;
			for (size_t i = 0; i < ws.mMapData.Height();++i)
			for (size_t j = 0; j < ws.mMapData.Width(); ++j)
			{
				// Instantiate archetypes!
				auto& v = ws.mMapData(j, i);
				if (v & (rl::eMapData::corridor | rl::eMapData::room | rl::eMapData::conn))
					mBgEntities(j, i) = it_floor;
				if (v & rl::eMapData::conn)
					mFgEntities(j, i) = ecs->Create( it_door->second );
				if (v & rl::eMapData::entry)
				{
					it_entry_inst = ecs->Create(it_enter->second);
					mFgEntities(j, i) = it_entry_inst;
					mEntry = glm::ivec2(j, i);
				}
				if (v & rl::eMapData::exit)
				{
					mFgEntities(j, i) = ecs->Create(it_exit->second);
					mExit = glm::ivec2(j, i);
				}		
			}

			// Init obstacles
			UpdateStaticMoveCosts();
			UpdateObstacles();
		}

		//-------------------------------------------------------------------------------
		void cLayout::SetActor(ecs::cEntityWithDataConst ed)
		{
			auto loc = ed->second.Component<ecs::cmp::cLocation>();
			mActors[ed] = loc->mPos;
		}

		//-------------------------------------------------------------------------------
		void cLayout::RemoveActor(ecs::cEntityWithDataConst ed)
		{
			auto loc = ed->second.Component<ecs::cmp::cLocation>();
			mActors.erase(ed);
		}

		//-------------------------------------------------------------------------------
		void cLayout::UpdateStaticMoveCosts()
		{
			// TODO: too expensive for opening doors!
			mStaticMoveCosts.Resize(BgEntities().Width(), BgEntities().Height());
			BgEntities().View().VisitRext([&](size_t x, size_t y, const ecs::cArchetypeWithDataConst& ed){ mMoveCosts(x, y) = ed->second.Component<ecs::cmp::cMoveCost>()->mMoveCost; });
			mStaticMoveCosts.View().VisitWext([&](size_t x, size_t y, float& v){ v = std::max(BgEntities()(x, y)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost,
																					  	      FgEntities()(x, y)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost); });
		}
		//-------------------------------------------------------------------------------
		void cLayout::UpdateMoveCosts()
		{
			// TODO: Static + actors! update smartly
		}
		//-------------------------------------------------------------------------------
		void cLayout::UpdateObstacles()
		{
			mObstacles.Resize(BgEntities().Width(), BgEntities().Height());
			BgEntities().View().VisitRext([&](size_t x, size_t y, const ecs::cArchetypeWithDataConst& ed){ mObstacles(x, y) = ed->second.Component<ecs::cmp::cMoveCost>()->mMoveCost == std::numeric_limits<float>::max(); });
		}

		//-------------------------------------------------------------------------------
		void cLayout::UpdateLayout(ecs::cEntityWithDataConst ed, ecs::cmp::cLocation * zLocOld, ecs::cmp::cLocation * zLocNew)
		{
			// TODO: this looks like a nice event handle, but I don't want all levels to be listening to that stuff.
			if (!zLocOld)
				AddTile(ed, *zLocNew);
			else if (!zLocNew)
				RemoveTile(ed, *zLocOld);
			else
				UpdateTile(ed, *zLocOld, *zLocNew);
		}

		void cLayout::AddTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocNew)
		{
			// Add an enum for TileType: bg/fg/atmo/actor/ atmofg?
		}

		void cLayout::RemoveTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocOld)
		{

		}

		void cLayout::UpdateTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocOld, const ecs::cmp::cLocation& zLocNew)
		{
			
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