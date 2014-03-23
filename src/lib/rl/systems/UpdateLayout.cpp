#include "UpdateLayout.h"

#include <rl/event/events.h>
#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			bool cUpdateLayout::operator()(ecs::cEntityWithData& ed, cmp::cLocation * zOldLoc)
			{
				auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
				auto newloc = ed->second.Component<cmp::cLocation>();

				auto oldlvl_it = world->mLevelMap.find(zOldLoc->mLevelId);
				auto newlvl_it = world->mLevelMap.find(newloc->mLevelId);
				//rl::cLayout * oldlay = (oldlvl_it != world->mLevelMap.end()) ? &oldlvl_it->second->second.Component<cmp::cLevelData>()->mLayout : nullptr;
				//rl::cLayout * newlay = (newlvl_it != world->mLevelMap.end()) ? &newlvl_it->second->second.Component<cmp::cLevelData>()->mLayout : nullptr;

				// level didn't change
				if (zOldLoc->mLevelId == newloc->mLevelId)
				{
					if (newlvl_it != world->mLevelMap.end())
						UpdateLayout(ed, newlvl_it->second, zOldLoc->mPos);
				}
				else // level changed
				{
					if (oldlvl_it != world->mLevelMap.end())
						RemoveFromLayout(ed, oldlvl_it->second, zOldLoc->mPos);
					if (newlvl_it != world->mLevelMap.end())
						AddToLayout(ed, newlvl_it->second);
				}
				return true;
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::AddToLayout(ecs::cEntityWithData& ed, ecs::cEntityWithData lvl)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				const auto& pos = ed->second.Component<cmp::cLocation>()->mPos;
				auto tileType = ed->second.Component<ecs::cmp::cTileInfo>()->mType;
				switch (tileType)
				{
				case ecs::cmp::eTileGroup::Bg:
					lay.mBg.Add(ed, pos);
					break;
				case ecs::cmp::eTileGroup::Fg:
					lay.mFg.Add(ed, pos);
					UpdateStaticMoveCosts(lvl, pos);
					break;
				case ecs::cmp::eTileGroup::Atmo:
				case ecs::cmp::eTileGroup::Actor:
					lay.mActors.Add(ed, pos);
					UpdateMoveCosts(lvl,pos);
					break;
				default:
					break;
				}
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::RemoveFromLayout(ecs::cEntityWithData& ed, ecs::cEntityWithData lvl, const glm::ivec2& oldPos)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				auto tileType = ed->second.Component<ecs::cmp::cTileInfo>()->mType;
				switch (tileType)
				{
				case ecs::cmp::eTileGroup::Bg:
					lay.mBg.Remove(ed, oldPos);
					break;
				case ecs::cmp::eTileGroup::Fg:
					lay.mFg.Remove(ed, oldPos);
					UpdateStaticMoveCosts(lvl,oldPos);
					break;
				case ecs::cmp::eTileGroup::Atmo:
				case ecs::cmp::eTileGroup::Actor:
					lay.mActors.Remove(ed, oldPos);
					UpdateMoveCosts(lvl,oldPos);
					break;
				default:
					break;
				}
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::UpdateLayout(ecs::cEntityWithData& ed, ecs::cEntityWithData lvl, const glm::ivec2& oldPos)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				const auto& pos = ed->second.Component<cmp::cLocation>()->mPos;
				auto tileType = ed->second.Component<ecs::cmp::cTileInfo>()->mType;
				switch (tileType)
				{
				case ecs::cmp::eTileGroup::Bg:
					// should not really happen
					assert(false);
					lay.mBg.Move(ed, oldPos, pos);
					break;
				case ecs::cmp::eTileGroup::Fg:
					lay.mFg.Move(ed, oldPos, pos);
					UpdateStaticMoveCosts(lvl,oldPos);
					UpdateStaticMoveCosts(lvl, pos);
					break;
				case ecs::cmp::eTileGroup::Atmo:
				case ecs::cmp::eTileGroup::Actor:
					lay.mActors.Move(ed, oldPos, pos);
					UpdateMoveCosts(lvl,oldPos);
					UpdateMoveCosts(lvl, pos);
					break;
				default:
					break;
				}
			}

			//------------------------------------------------------------------------------------------

			void cUpdateLayout::GenerateStaticMoveCosts(ecs::cEntityWithData lvl)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				lay.mStaticMoveCosts.Resize(lay.mDims.x, lay.mDims.y);
				lay.mBg.Cells().View().VisitRext([&](size_t x, size_t y, const ecs::cEntityWithDataConst& ed){ lay.mStaticMoveCosts(x, y) = ed->second.Component<ecs::cmp::cMoveCost>()->mMoveCost; });
				if (!lay.mFg.Entities().empty())
				for (const auto& v : lay.mFg.Entities())
					lay.mStaticMoveCosts(v->second.Component<ecs::cmp::cLocation>()->mPos) = v->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
				// TODO: emit event
				GenerateMoveCosts(lvl);
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::GenerateMoveCosts(ecs::cEntityWithData lvl)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				lay.mMoveCosts = lay.mStaticMoveCosts;
				if (!lay.mActors.Entities().empty())
				for (const auto& v : lay.mActors.Entities())
				{
					const auto& pos = v->second.Component<ecs::cmp::cLocation>()->mPos;
					lay.mMoveCosts(pos) = std::max(v->second.Component<ecs::cmp::cMoveCost>()->mMoveCost, lay.mStaticMoveCosts(pos));
				}
				// TODO: emit event
				GenerateObstacles(lvl);
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::GenerateObstacles(ecs::cEntityWithData lvl)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				lay.mObstacles.Resize(lay.mDims.x, lay.mDims.y);
				lay.mMoveCosts.View().VisitRext([&](size_t x, size_t y, float v){ lay.mObstacles(x, y) = (v == std::numeric_limits<float>::max()); });
				// TODO: emit event
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::UpdateStaticMoveCosts(ecs::cEntityWithData lvl, const glm::ivec2& pos)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				auto cost_bg = lay.mBg.Cells()(pos)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
				auto cost_fg = lay.mFg.Cells().View().IsClear(pos.x, pos.y) ? 0.0f : lay.mFg.Cells()(pos)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
				auto cost = std::max(cost_bg, cost_fg);
				if (lay.mStaticMoveCosts(pos) != cost)
				{
					lay.mStaticMoveCosts(pos) = cost;
					// TODO: emit event
					UpdateMoveCosts(lvl, pos);
				}
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::UpdateMoveCosts(ecs::cEntityWithData lvl, const glm::ivec2& pos)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				auto cost_act = lay.mActors.Cells().View().IsClear(pos.x, pos.y) ? 0.0f : lay.mActors.Cells()(pos)->second.Component<ecs::cmp::cMoveCost>()->mMoveCost;
				if (lay.mMoveCosts(pos) != cost_act)
				{
					lay.mMoveCosts(pos) = cost_act;
					// TODO: emit event
					UpdateObstacles(lvl, pos);
				}
			}

			//------------------------------------------------------------------------------------------
			void cUpdateLayout::UpdateObstacles(ecs::cEntityWithData lvl, const glm::ivec2& pos)
			{
				rl::cLayout& lay = lvl->second.Component<cmp::cLevelData>()->mLayout;
				lay.mObstacles(pos) = lay.mMoveCosts(pos) == std::numeric_limits<float>::max();
				// TODO: emit event
			}
		}
	}
}