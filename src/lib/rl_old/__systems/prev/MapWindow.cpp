#include "MapWindow.h"

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/MapWindow.h>
#include <rl/components/TileLayout.h>
#include <rl/components/MapSprite.h>
#include <rl/components/LevelPosition.h>

namespace pgn
{
	namespace sys
	{
		cMapWindow::cMapWindow()
		:mOnLevelLoaded(Simple::slot(this, &cMapWindow::OnLevelLoaded))
		,mOnLevelUnload(Simple::slot(this, &cMapWindow::OnLevelUnload))
		{

		}

		void cMapWindow::OnLevelLoaded(cEntityWithData ed)
		{ 
			std::shared_ptr< cComponent<cmp::cTileLayout>> map_ptr;
			ed->second.mComponents.GetComponent(map_ptr);
			auto& curmap = map_ptr->mData;
			auto& mapdata = curmap.mData;

			for (auto e : mQueryMapWindow->Entities())
			{
				const auto& edmw = ECS.mEntityMgr->GetEntityData(e);
				std::shared_ptr< cComponent<cmp::cMapWindow>> mwin_ptr;
				edmw.mComponents.GetComponent(mwin_ptr);
				auto & mwin = mwin_ptr->mData;	

				
				for (unsigned i = 0; i < mapdata.Height();++i)
					for (unsigned j = 0; j < mapdata.Width(); ++j)
					{
						// get entity from 2d map
						auto tile = mapdata(j, i);

						// get sprite from entity
						std::shared_ptr< cComponent<cmp::cMapSprite>> sprite_ptr;
						tile->second.mComponents.GetComponent(sprite_ptr);
						
						// assign sprite
						mwin.Tile(j, i)->setAnimFrame(sprite_ptr->mData.mSprite->getAnimFrame());
						mwin.Tile(j, i)->setName(sprite_ptr->mData.mSprite->getName());
					}

				// TODO: attach all tiles to mapwindow actor or root.
			}

			// TODO: Get player and position him in center of map. Do that to sprite too?
		}

		void cMapWindow::OnLevelUnload(cEntityWithData ed)
		{
			for (auto e : mQueryMapWindow->Entities())
			{
			}
		}
	}
}