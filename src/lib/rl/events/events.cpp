#include "events.h"

#include <ecs/ecs.h>
#include <ecs/SystemMgr.h>

#include <rl/components/components.h>
#include <rl/components/TileLayout.h>
#include <rl/components/MapSprite.h>
#include <rl/components/MapWindow.h>
#include <rl/components/Movement.h>
#include <rl/components/Door.h>
#include <rl/components/Description.h>
#include <rl/components/KeyActionMapper.h>
#include <rl/components/LevelPosition.h>
#include <rl/components/Log.h>
#include <rl/components/OutStream.h>
#include <rl/components/TextWindow.h>
#include <rl/components/TileObstacle.h>

static const char * dir2text_full[] = { "south-west", "south", "south-east", "west", "nowhere", "east", "north-west", "north", "north-east" };

namespace pgn
{
	namespace evt
	{
		//----------------------------------------------------------------------------------------------------
		void OnLevelCreated(cEntityWithData arg0)
		{
			assert(false);
		}
		//----------------------------------------------------------------------------------------------------
		void OnLevelDestroy(cEntityWithData arg0)
		{
			assert(false);
		}
		//----------------------------------------------------------------------------------------------------
		void OnLevelLoaded(cEntityWithData ed)
		{
			std::shared_ptr< cComponent<pgn::cmp::cTileLayout>> map_ptr;
			ed->second.mComponents.GetComponent(map_ptr);
			auto& curmap = map_ptr->mData;
			auto& mapdata = curmap.mData;
            
            auto queryWin = ECS.mSystemMgr->GetQuery("pgn::cmp::cMapWindow");

			for (auto e : queryWin->Entities())
			{
				const auto& edmw = ECS.mEntityMgr->GetEntityData(e);
				std::shared_ptr< cComponent<pgn::cmp::cMapWindow>> mwin_ptr;
				edmw.mComponents.GetComponent(mwin_ptr);
				auto & mwin = mwin_ptr->mData;	

				
				for (unsigned i = 0; i < mapdata.Height();++i)
					for (unsigned j = 0; j < mapdata.Width(); ++j)
					{
						// get entity from 2d map
						auto tile = mapdata(j, i);

						// get sprite from entity
						std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
						tile->second.mComponents.GetComponent(sprite_ptr);
						
						// assign sprite
						mwin.Tile(j, i)->setAnimFrame(sprite_ptr->mData.mSprite->getAnimFrame());
						mwin.Tile(j, i)->setName(sprite_ptr->mData.mSprite->getName());
					}

				// TODO: attach all tiles to mapwindow actor or root.
			}

			// TODO: Get player and position him in center of map. Do that to sprite too?
		}
		//----------------------------------------------------------------------------------------------------
		void OnLevelUnload(cEntityWithData arg0)
		{
			assert(false);
		}
		//----------------------------------------------------------------------------------------------------
		void OnExitApplication()
		{
			assert(false);
		}
		//----------------------------------------------------------------------------------------------------
		void OnLog(const std::string& zLoggerName, const std::string& zString)
		{
            auto queryLog = ECS.mSystemMgr->GetQuery("pgn::cmp::cLog");
            auto queryLogListener = ECS.mSystemMgr->GetQuery("tag:log:"+zLoggerName);
			for (auto e : queryLog->Entities())
			{
				// Get the log
				std::shared_ptr< cComponent<pgn::cmp::cLog>> log_ptr;
				auto ec = ECS.mEntityMgr->GetEntityData().find(e);
				assert(ec != ECS.mEntityMgr->GetEntityData().end());
				
				// Move to the next logger if it's not the one we're looking for
				if (ec->second.mName != zLoggerName)
					continue;

				// Get the log
				ec->second.mComponents.GetComponent(log_ptr);
				pgn::cmp::cLog& a_log = log_ptr->mData;

				// Do we have available lines to write?
				if (a_log.mMaxLineNum)
				{
					// if we're full, erase the oldest line
					if (a_log.mLines.size() == a_log.mMaxLineNum)
						a_log.mLines.erase(a_log.mLines.begin());

					// TODO: apply modifiers here, depending on the type of the logger. virtual function for text transfoermation

					// write the log line
					a_log.mLines.push_back(zString);
				}

				// Convert all log lines to a single string for update
				auto log_string = pystring::join("\n", a_log.mLines);
                
				// Find out all the listeners
				for (auto e : queryLogListener->Entities())
				{
					auto ec = ECS.mEntityMgr->GetEntityData().find(e);
					assert(ec != ECS.mEntityMgr->GetEntityData().end());

					// Does it have a TextWin?
					std::shared_ptr< cComponent<pgn::cmp::cTextWindow>> twin_ptr;
					ec->second.mComponents.GetComponent(twin_ptr);
					if (twin_ptr)
					{
						twin_ptr->mData.SetText(log_string);
					}

					// Does it have an OutStream?
					std::shared_ptr< cComponent<pgn::cmp::cOutStream>> os_ptr;
					ec->second.mComponents.GetComponent(os_ptr);
					if (os_ptr)
					{

					}
				}
			}
		}
		//----------------------------------------------------------------------------------------------------
		void OnActionIdle(cEntityWithData ed)
		{
			evt::cLog::mSig.emit("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[4]));
		}
		//----------------------------------------------------------------------------------------------------
		void OnActionMoveAdj(cEntityWithData ed, const glm::ivec2& vin)
		{
			glm::ivec2 v = vin;

			// Get entity data
			std::shared_ptr< cComponent<pgn::cmp::cMovement>> move_ptr;
			std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
			std::shared_ptr< cComponent<pgn::cmp::cLevelPosition>> pos_ptr;
			ed->second.mComponents.GetComponent(move_ptr);
			ed->second.mComponents.GetComponent(sprite_ptr);
			ed->second.mComponents.GetComponent(pos_ptr);

			// TODO: apply logic to check if we can move, apply movepoint reduction etc.

			// TODO: temp!
			pos_ptr->mData.mPos += v;
			auto curspritepos = sprite_ptr->mData.mSprite->getPosition();
			sprite_ptr->mData.mSprite->setPosition(curspritepos + oxygine::Vector2(32.0f * v.x, -32.0f * v.y));

			// Log
			int udir = v.x + 1 + (v.y + 1) * 3;
			evt::cLog::mSig.emit("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[udir]));
		}
		//----------------------------------------------------------------------------------------------------
		void OnActionDoorOpen(cEntityWithData ewho, cEntityWithData ed)
		{
			std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
			ed->second.mComponents.GetComponent(door_ptr);
			if (door_ptr->mData.mIsClosed)
			{
				door_ptr->mData.mIsClosed = false;
				evt::cDoorOpened::mSig.emit(ed);
			}
		}
		//----------------------------------------------------------------------------------------------------
		void OnActionDoorClose(cEntityWithData ewho, cEntityWithData ed)
		{
			std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
			ed->second.mComponents.GetComponent(door_ptr);
			if (door_ptr->mData.mIsClosed)
			{
				door_ptr->mData.mIsClosed = true;
				evt::cDoorClosed::mSig.emit(ed);
			}
		}
		//----------------------------------------------------------------------------------------------------
		void OnDoorOpened(cEntityWithData ed)
		{
			std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
			std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
			ed->second.mComponents.GetComponent(door_ptr);
			ed->second.mComponents.GetComponent(sprite_ptr);
			sprite_ptr->mData = door_ptr->mData.mSprites[0];
			// TODO: update mapwin's tiles? Above won't work unless I attach stuff to scenegraph
		}
		//----------------------------------------------------------------------------------------------------
		void OnDoorClosed(cEntityWithData ed)
		{
			std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
			std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
			ed->second.mComponents.GetComponent(door_ptr);
			ed->second.mComponents.GetComponent(sprite_ptr);
			sprite_ptr->mData = door_ptr->mData.mSprites[1];
			// TODO: update mapwin's tiles? Above won't work unless I attach stuff to scenegraph
		}
		//----------------------------------------------------------------------------------------------------
		void OnTileInLevelChanged(cEntityWithData arg0)
		{
			assert(false);
		}
        
        //----------------------------------------------------------------------------------------------------
		void OnKeyState(const int key, const oxygine::cKeyState& state)
		{
			// Debug key presses
			//std::cout << boost::str(boost::format("%s - %s - %d\n")% (state.mPressed ? "pressed" : "released")%(state.mRepeat ? "repeat" : "no-repeat")%key );
            
            auto queryKam = ECS.mSystemMgr->GetQuery("pgn::cmp::cKeyActionMapper");

			for (auto e : queryKam->Entities())
			{
				// Get the action mapper
				std::shared_ptr< cComponent<pgn::cmp::cKeyActionMapper>> kam_ptr;
				auto ec = ECS.mEntityMgr->GetEntityData().find(e);
				assert(ec != ECS.mEntityMgr->GetEntityData().end());
				ec->second.mComponents.GetComponent(kam_ptr);
				pgn::cmp::cKeyActionMapper& kam = kam_ptr->mData;
				
				// if pressed
				if (state.mPressed)
				{
					// if action is found
					auto it = kam.mActions.find(key);
					if (it != kam.mActions.end())
						// execute action!
						it->second();
				}
			}

            auto queryDevC = ECS.mSystemMgr->GetQuery("pgn::cmp::cDevConsole");
			for (auto e : queryDevC->Entities())
			{
				// TODO: ... 
			}
		}
	}
}