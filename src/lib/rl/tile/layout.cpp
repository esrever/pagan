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
					// TODO: prime for being a helper
					ed->second.AddComponent(ecs::cComponent<ecs::cmp::cLocation>::Create());
					ed->second.Component<ecs::cmp::cLocation>()->mPos = pos;
					mFg.Add(ed, pos);
				}
				if (v & rl::eMapData::entry)
				{
					auto ed = mainecs()->InstantiateArchetype(it_enter->second);
					// TODO: prime for being a helper
					ed->second.AddComponent(ecs::cComponent<ecs::cmp::cLocation>::Create());
					ed->second.Component<ecs::cmp::cLocation>()->mPos = pos;
					mFg.Add(ed, pos);
					mEntry = pos;
				}
				if (v & rl::eMapData::exit)
				{
					auto ed = mainecs()->InstantiateArchetype(it_exit->second);
					// TODO: prime for being a helper
					ed->second.AddComponent(ecs::cComponent<ecs::cmp::cLocation>::Create());
					ed->second.Component<ecs::cmp::cLocation>()->mPos = pos;
					mFg.Add(ed, pos);
					mExit = pos;
				}		
			}

			// Init obstacles
			UpdateStaticMoveCosts();
		}

		//-------------------------------------------------------------------------------
		void cLayout::UpdateStaticMoveCosts()
		{
			mStaticMoveCosts.Resize(mDims.x, mDims.y);
			Bg().Cells().View().VisitRext([&](size_t x, size_t y, const ecs::cEntityWithDataConst& ed){ mStaticMoveCosts(x, y) = ed->second.Component<ecs::cmp::cMoveCost>()->mMoveCost; });
			if (!Fg().Entities().empty())
				for (const auto& v : Fg().Entities())
					mStaticMoveCosts(v->second.Component<ecs::cmp::cLocation>()->mPos) = v->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
			UpdateMoveCosts();
			// TODO: emit event. btu where's the levle entiti?
		}
		//-------------------------------------------------------------------------------
		void cLayout::UpdateMoveCosts()
		{
			mMoveCosts = mStaticMoveCosts;
			if (!Actors().Entities().empty())
			for (const auto& v : Actors().Entities())
			{
				const auto& pos = v->second.Component<ecs::cmp::cLocation>()->mPos;
				mMoveCosts(pos) = std::max(v->second.Component<ecs::cmp::cMoveCost>()->mMoveCost, mStaticMoveCosts(pos));
			}
			UpdateObstacles();
			// TODO: emit event. btu where's the levle entiti?
		}
		//-------------------------------------------------------------------------------
		void cLayout::UpdateObstacles()
		{
			mObstacles.Resize(mDims.x, mDims.y);
			mMoveCosts.View().VisitRext([&](size_t x, size_t y, float v){ mObstacles(x, y) = (v == std::numeric_limits<float>::max()); });
		}

		//-------------------------------------------------------------------------------
		void cLayout::UpdateStaticMoveCost(const glm::ivec2& pos)
		{
			// TODO: better fg handle
			auto cost_bg = Bg().Cells()(pos)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
			auto cost_fg = Fg().Cells().View().IsClear(pos.x, pos.y) ? 0.0f : Fg().Cells()(pos)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
			auto cost = std::max(cost_bg, cost_fg);
			if (mStaticMoveCosts(pos) != cost)
			{
				mStaticMoveCosts(pos) = cost;
				UpdateMoveCost(pos);
			}
		}

		//-------------------------------------------------------------------------------
		void cLayout::UpdateMoveCost(const glm::ivec2& pos)
		{
			auto cost_act = Actors().Cells().View().IsClear(pos.x, pos.y) ? 0.0f : Actors().Cells()(pos)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
			if (mMoveCosts(pos) != cost_act)
			{
				mMoveCosts(pos) = cost_act;
				UpdateObstacle(pos);
			}
		}

		//-------------------------------------------------------------------------------
		void cLayout::UpdateObstacle(const glm::ivec2& pos)
		{
			mObstacles(pos) = mMoveCosts(pos) == std::numeric_limits<float>::max();
		}


		//-------------------------------------------------------------------------------
		void cLayout::UpdateLayout(ecs::cEntityWithDataConst ed, ecs::cmp::cLocation * zLocOld, ecs::cmp::cLocation * zLocNew)
		{
			if (!zLocOld)
				AddTile(ed, *zLocNew);
			else if (!zLocNew)
				RemoveTile(ed, *zLocOld);
			else
				UpdateTile(ed, *zLocOld, *zLocNew);
		}

		void cLayout::AddTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocNew)
		{
			auto tileType = ed->second.Component<ecs::cmp::cTileInfo>()->mType;
			switch (tileType)
			{
			case ecs::cmp::eTileGroup::Bg:
				Bg().Add(ed, zLocNew.mPos);
				break;
			case ecs::cmp::eTileGroup::Fg:
				Fg().Add(ed, zLocNew.mPos);
				UpdateStaticMoveCost(zLocNew.mPos);
				break;
			case ecs::cmp::eTileGroup::Atmo:
			case ecs::cmp::eTileGroup::Actor:
				Actors().Add(ed, zLocNew.mPos);
				UpdateMoveCost(zLocNew.mPos);
				break;
			default:
				break;
			}
		}

		void cLayout::RemoveTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocOld)
		{
			auto tileType = ed->second.Component<ecs::cmp::cTileInfo>()->mType;
			switch (tileType)
			{
			case ecs::cmp::eTileGroup::Bg:
				Bg().Remove(ed, zLocOld.mPos);
				break;
			case ecs::cmp::eTileGroup::Fg:
				Fg().Remove(ed, zLocOld.mPos);
				UpdateStaticMoveCost(zLocOld.mPos);
				break;
			case ecs::cmp::eTileGroup::Atmo:
			case ecs::cmp::eTileGroup::Actor:
				Actors().Remove(ed, zLocOld.mPos);
				UpdateMoveCost(zLocOld.mPos);
				break;
			default:
				break;
			}
		}

		void cLayout::UpdateTile(ecs::cEntityWithDataConst ed, const ecs::cmp::cLocation& zLocOld, const ecs::cmp::cLocation& zLocNew)
		{
			auto tileType = ed->second.Component<ecs::cmp::cTileInfo>()->mType;
			switch (tileType)
			{
			case ecs::cmp::eTileGroup::Bg:
				// TODO: this is kinda weird. should not really happen
				assert(false);
				Bg().Move(ed, zLocOld.mPos, zLocNew.mPos);
				break;
			case ecs::cmp::eTileGroup::Fg:
				Fg().Move(ed, zLocOld.mPos, zLocNew.mPos);
				UpdateStaticMoveCost(zLocOld.mPos);
				UpdateStaticMoveCost(zLocNew.mPos);
				break;
			case ecs::cmp::eTileGroup::Atmo:
			case ecs::cmp::eTileGroup::Actor:
				Actors().Move(ed, zLocOld.mPos, zLocNew.mPos);
				UpdateMoveCost(zLocOld.mPos);
				UpdateMoveCost(zLocNew.mPos);
				break;
			default:
				break;
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