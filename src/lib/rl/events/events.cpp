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
#include <rl/components/Level.h>
#include <rl/components/Log.h>
#include <rl/components/OutStream.h>
#include <rl/components/TextWindow.h>
#include <rl/components/TileObstacle.h>

static const char * dir2text_full[] = { "south-west", "south", "south-east", "west", "nowhere", "east", "north-west", "north", "north-east" };

namespace pgn
{
	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LEVEL_CREATE), cEntityWithData>::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LEVEL_CREATE), cEntityWithData>::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Event");

		std::shared_ptr< cComponent<pgn::cmp::cTileLayout>> layout_ptr;
		std::shared_ptr< cComponent<pgn::cmp::cLevel>> lvl_ptr;
		ed->second.mComponents.GetComponent(layout_ptr);
		ed->second.mComponents.GetComponent(lvl_ptr);
		auto& curmap = layout_ptr->mData;
		auto& mapdata = curmap.mData;

		// Layout response: attach layout node to level node
		layout_ptr->mData.mLayoutNode->attachTo(lvl_ptr->mData.mLevelNode);
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LEVEL_DESTROY), cEntityWithData>::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelDestroy>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LEVEL_DESTROY), cEntityWithData>::Event(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Event");
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LEVEL_LOAD), cEntityWithData>::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLoad>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LEVEL_LOAD), cEntityWithData>::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLoad>::Run");

		// TODO: Check if we have another level loaded, and unload if we do
		auto queryLvl = ECS.mSystemMgr->GetQuery("tag:CurrentLevel");

		// Get the tile layout & level from the input
		std::shared_ptr< cComponent<pgn::cmp::cTileLayout>> layout_ptr;
		std::shared_ptr< cComponent<pgn::cmp::cLevel>> lvl_ptr;
		ed->second.mComponents.GetComponent(layout_ptr);
		ed->second.mComponents.GetComponent(lvl_ptr);
		auto& curmap = layout_ptr->mData;
		auto& mapdata = curmap.mData;

		// Mapwindow response
		auto e = ECS.mEntityMgr->Globals().mMapWindow;
		std::shared_ptr< cComponent<pgn::cmp::cMapWindow>> mwin_ptr;
		e->second.mComponents.GetComponent(mwin_ptr);
		auto & mwin = mwin_ptr->mData;

		// Attach level node to window node
		lvl_ptr->mData.mLevelNode->attachTo(mwin.mTileWin);

		// for each tile // TODO: use visitor instead? 
		for (unsigned i = 0; i < mapdata.Height(); ++i)
		{
			for (unsigned j = 0; j < mapdata.Width(); ++j)
			{
				// get entity from 2d map
				auto tile = mapdata(j, i);

				// get sprite from entity
				std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
				std::shared_ptr< cComponent<pgn::cmp::cLevelPosition>> pos_ptr;
				tile->second.mComponents.GetComponent(sprite_ptr);
				tile->second.mComponents.GetComponent(pos_ptr);
				pos_ptr->mData.mLevel = ed;
				sprite_ptr->mData.mSprite->setSize(float(mwin.mTileDims.x), float(mwin.mTileDims.y));
				sprite_ptr->mData.mSprite->setPosition(mwin.LevelToScreenCoords(pos_ptr->mData));
			}
		}
		
        // Put pc on a valid square
		auto epc = ECS.mEntityMgr->Globals().mPC;
		cActionLocationChange::RunEvent(epc, ed, GetRandomStartPos(layout_ptr->mData));
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LEVEL_UNLOAD), cEntityWithData>::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelUnload>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LEVEL_UNLOAD), cEntityWithData>::Event(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Event");
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LEVEL_ENTER), cEntityWithData, cEntityWithData>::Run(cEntityWithData arg0, cEntityWithData arg1)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelEnter>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LEVEL_ENTER), cEntityWithData, cEntityWithData>::Event(cEntityWithData ewho, cEntityWithData elvl)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelEnter>::Event");

		std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
		std::shared_ptr< cComponent<pgn::cmp::cLevelPosition>> pos_ptr;
		ewho->second.mComponents.GetComponent(sprite_ptr);
		ewho->second.mComponents.GetComponent(pos_ptr);
		pos_ptr->mData.mLevel = elvl;
		std::shared_ptr< cComponent<pgn::cmp::cLevel>> level_ptr;
		elvl->second.mComponents.GetComponent(level_ptr);
		if (level_ptr->mData.mLevelNode != sprite_ptr->mData.mSprite->getParent())
			sprite_ptr->mData.mSprite->attachTo(level_ptr->mData.mLevelNode);
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LEVEL_LEAVE), cEntityWithData, cEntityWithData>::Run(cEntityWithData arg0, cEntityWithData arg1)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLeave>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LEVEL_LEAVE), cEntityWithData, cEntityWithData>::Event(cEntityWithData arg0, cEntityWithData arg1)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLeave>::Event");
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::EXIT_APPLICATION)>::Run()
	{
		cActionLog::RunEvent("system_log", "Called cAction<ExitApplication>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::EXIT_APPLICATION)>::Event()
	{
		cActionLog::RunEvent("system_log", "Called cAction<ExitApplication>::Run");
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LOG), const std::string&, const std::string&>::Run(const std::string& zLoggerName, const std::string& zString)
	{
		return true;
		auto queryLog = ECS.mSystemMgr->GetQuery("pgn::cmp::cLog");
		auto queryLogListener = ECS.mSystemMgr->GetQuery("tag:log:" + zLoggerName);
		bool ok = true;
		for (auto ec : queryLog->Entities())
		{
			// Get the log
			std::shared_ptr< cComponent<pgn::cmp::cLog>> log_ptr;

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
			else
				ok = false;

			// Convert all log lines to a single string for update
			auto log_string = pystring::join("\n", a_log.mLines);

			// Find out all the listeners
			for (auto ec : queryLogListener->Entities())
			{
				// Does it have a TextWin?
				std::shared_ptr< cComponent<pgn::cmp::cTextWindow>> twin_ptr;
				ec->second.mComponents.GetComponent(twin_ptr);
				if (twin_ptr)
				{
					twin_ptr->mData.SetText(log_string);
				}

				// Does it have an OutStream?
				/*
				std::shared_ptr< cComponent<pgn::cmp::cOutStream>> os_ptr;
				ec->second.mComponents.GetComponent(os_ptr);
				if (os_ptr)
				{

				}
				*/
			}
		}
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LOG), const std::string&, const std::string&>::Event(const std::string& arg0, const std::string& arg1)
	{
		//assert(false);
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::ACTION_IDLE), cEntityWithData>::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionIdle>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::ACTION_IDLE), cEntityWithData>::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionIdle>::Event");
		cActionLog::RunEvent("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[4]));
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::ACTION_MOVE_ADJ), cEntityWithData, const glm::ivec2&>::Run(cEntityWithData ed, const glm::ivec2& vin)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionMoveAdj>::Run");

		glm::ivec2 v = vin;

		// Get entity data
		std::shared_ptr< cComponent<pgn::cmp::cMovement>> move_ptr;
		std::shared_ptr< cComponent<pgn::cmp::cLevelPosition>> pos_ptr;
		ed->second.mComponents.GetComponent(move_ptr);
		ed->second.mComponents.GetComponent(pos_ptr);

        // Get layout of level
		std::shared_ptr< cComponent<pgn::cmp::cTileLayout>> layout_ptr;
		pos_ptr->mData.mLevel->second.mComponents.GetComponent(layout_ptr);
		const auto& mapdata = layout_ptr->mData.mData;
		
        // if the new position is within the bounds of the level
		const glm::ivec2 newpos = pos_ptr->mData.mPos + v;
		if (mapdata.InRange(newpos))
		{
            // Check if we hit an obstacle
			std::shared_ptr< cComponent<pgn::cmp::cTileObstacle>> obstacle_ptr;
			mapdata(newpos)->second.mComponents.GetComponent(obstacle_ptr);
			if (!obstacle_ptr->mData.mIsObstacle)
				cActionLocationChange::RunEvent(ed, pos_ptr->mData.mLevel, newpos);
		}
		

		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::ACTION_MOVE_ADJ), cEntityWithData, const glm::ivec2&>::Event(cEntityWithData ed, const glm::ivec2& v)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionMoveAdj>::Event");

		// Log
		int udir = v.x + 1 + (v.y + 1) * 3;
		cActionLog::RunEvent("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[udir]));
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::DOOR_OPEN), cEntityWithData, cEntityWithData>::Run(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorOpen>::Run");

		std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
		ed->second.mComponents.GetComponent(door_ptr);
		bool ok = door_ptr->mData.mIsClosed;
		if (ok)
			door_ptr->mData.mIsClosed = false;
		return ok;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::DOOR_OPEN), cEntityWithData, cEntityWithData>::Event(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorOpen>::Event");

		std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
		std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
		ed->second.mComponents.GetComponent(door_ptr);
		ed->second.mComponents.GetComponent(sprite_ptr);
		sprite_ptr->mData.mSprite->setResAnim(door_ptr->mData.mSprites[0].mSprite->getResAnim());
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::DOOR_CLOSE), cEntityWithData, cEntityWithData>::Run(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorClose>::Run");

		std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
		ed->second.mComponents.GetComponent(door_ptr);
		bool ok = !door_ptr->mData.mIsClosed;
		if (ok)
			door_ptr->mData.mIsClosed = true;
		return ok;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::DOOR_CLOSE), cEntityWithData, cEntityWithData>::Event(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorClose>::Event");
		std::shared_ptr< cComponent<pgn::cmp::cDoor>> door_ptr;
		std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
		ed->second.mComponents.GetComponent(door_ptr);
		ed->second.mComponents.GetComponent(sprite_ptr);
		sprite_ptr->mData.mSprite->setResAnim(door_ptr->mData.mSprites[1].mSprite->getResAnim());
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::KEY_STATE), const int, const oxygine::cKeyState&>::Run(const int key, const oxygine::cKeyState& state)
	{
		cActionLog::RunEvent("system_log", "Called cAction<KeyState>::Run");

		// Debug key presses
		//std::cout << boost::str(boost::format("%s - %s - %d\n")% (state.mPressed ? "pressed" : "released")%(state.mRepeat ? "repeat" : "no-repeat")%key );

		auto queryKam = ECS.mSystemMgr->GetQuery("pgn::cmp::cKeyActionMapper");

		for (auto ec : queryKam->Entities())
		{
			// Get the action mapper
			std::shared_ptr< cComponent<pgn::cmp::cKeyActionMapper>> kam_ptr;
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

		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::KEY_STATE), const int, const oxygine::cKeyState&>::Event(const int, const oxygine::cKeyState&)
	{
		cActionLog::RunEvent("system_log", "Called cAction<KeyState>::Event");
		//assert(false);
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eBasicECS::ENTITY_CREATED), cEntityWithData>::Run(cEntityWithData)
	{
		cActionLog::RunEvent("system_log", "Called cAction<EntityCreated>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eBasicECS::ENTITY_CREATED), cEntityWithData>::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<EntityCreated>::Event");		
		
		// Set render priority dep. on type
		std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
		ed->second.mComponents.GetComponent(sprite_ptr);
		if (sprite_ptr)
		{
			if (ed->second.mTags.find("type:tile_bg") != ed->second.mTags.end())
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::DungeonElementBG;
			else if (ed->second.mTags.find("type:creature") != ed->second.mTags.end())
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::Creature;
			else
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::DungeonElementBG;
			sprite_ptr->mData.mSprite->setPriority(to_integral(sprite_ptr->mData.mRenderPriority));
		}
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cAction<size_t(evt::eRL::LOCATION_CHANGE), cEntityWithData, cEntityWithData, const glm::ivec2&>::Run(cEntityWithData ewho, cEntityWithData elvl, const glm::ivec2& pos)
	{	
		std::shared_ptr< cComponent<pgn::cmp::cLevelPosition>> pos_ptr;
		ewho->second.mComponents.GetComponent(pos_ptr);
		pos_ptr->mData.mPos = pos;
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cAction<size_t(evt::eRL::LOCATION_CHANGE), cEntityWithData, cEntityWithData, const glm::ivec2&>::Event(cEntityWithData ewho, cEntityWithData elvl, const glm::ivec2& pos)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LocationChange>::Run");

		std::shared_ptr< cComponent<pgn::cmp::cMapSprite>> sprite_ptr;
		std::shared_ptr< cComponent<pgn::cmp::cLevelPosition>> pos_ptr;
		ewho->second.mComponents.GetComponent(sprite_ptr);
		ewho->second.mComponents.GetComponent(pos_ptr);
		auto e = ECS.mEntityMgr->Globals().mMapWindow;
		std::shared_ptr< cComponent<pgn::cmp::cMapWindow>> mwin_ptr;
		e->second.mComponents.GetComponent(mwin_ptr);
		auto & mwin = mwin_ptr->mData;
		sprite_ptr->mData.mSprite->setPosition(mwin.LevelToScreenCoords(pos_ptr->mData));

		// Attach to level
		if (pos_ptr->mData.mLevel != elvl)
		{
			cActionLevelLeave::RunEvent(ewho, pos_ptr->mData.mLevel);
			cActionLevelEnter::RunEvent(ewho, elvl);
		}
	}

}