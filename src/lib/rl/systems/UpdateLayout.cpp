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
				rl::cLayout * oldlay = (oldlvl_it != world->mLevelMap.end()) ? &oldlvl_it->second->second.Component<cmp::cLevelData>()->mLayout : nullptr;
				rl::cLayout * newlay = (newlvl_it != world->mLevelMap.end()) ? &newlvl_it->second->second.Component<cmp::cLevelData>()->mLayout : nullptr;

				// level didn't change
				if (zOldLoc->mLevelId == newloc->mLevelId)
				{
					if (newlay)
					{
						if (zOldLoc->mPos != newloc->mPos)
							newlay->UpdateLayout(ed, zOldLoc, newloc);
					}
				}
				else // level changed
				{
					if (oldlay)
						oldlay->RemoveTile(ed, *zOldLoc);
					if (newlay)
						newlay->AddTile(ed, *newloc);
				}
				return true;
			}

			void cUpdateLayout::AddToLayout(ecs::cEntityWithData& ed, rl::cLayout * lay)
			{
				auto tileType = ed->second.Component<cmp::cTileInfo>()->mType;
				switch (tileType)
				{
					case cmp::eTileGroup::Bg:
					case cmp::eTileGroup::Fg:
					case cmp::eTileGroup::Atmo:
					case cmp::eTileGroup::Actor:
				default:
					break;
				}

			}

			void cUpdateLayout::RemoveFromLayout(ecs::cEntityWithData& ed, rl::cLayout * lay)
			{
				auto tileType = ed->second.Component<cmp::cTileInfo>()->mType;
				switch (tileType)
				{
				case cmp::eTileGroup::Bg:
				case cmp::eTileGroup::Fg:
				case cmp::eTileGroup::Atmo:
				case cmp::eTileGroup::Actor:
				default:
					break;
				}
			}

			void cUpdateLayout::UpdateLayout(ecs::cEntityWithData& ed, rl::cLayout * lay)
			{
				auto tileType = ed->second.Component<cmp::cTileInfo>()->mType;
				switch (tileType)
				{
				case cmp::eTileGroup::Bg:
				case cmp::eTileGroup::Fg:
				case cmp::eTileGroup::Atmo:
				case cmp::eTileGroup::Actor:
				default:
					break;
				}
			}
		}
	}
}