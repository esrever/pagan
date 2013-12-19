#include "events.h"

#include <glm/ext.hpp>

#include <ecs/ecs.h>
#include <ecs/SystemMgr.h>

#include <rl/util/FormatString.h>

#include <rl/components/components.h>
#include <rl/components/MapSprite.h>
#include <rl/components/MapWindow.h>
#include <rl/components/Movement.h>
#include <rl/components/Door.h>
#include <rl/components/Description.h>
#include <rl/components/GameStats.h>
#include <rl/components/KeyActionMapper.h>
#include <rl/components/LevelPosition.h>
#include <rl/components/Level.h>
#include <rl/components/Log.h>
#include <rl/components/OutStream.h>
#include <rl/components/FormatString.h>
#include <rl/components/TextWindow.h>
#include <rl/components/TileObstacle.h>

static const char * dir2text_full[] = { "south-west", "south", "south-east", "west", "nowhere", "east", "north-west", "north", "north-east" };

namespace pgn
{
	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLevelCreate::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionLevelCreate::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Event");
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLevelDestroy::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelDestroy>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionLevelDestroy::Event(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Event");
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLevelLoad::Run(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLoad>::Run");
		auto lvl_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cLevel>();
		auto mwin_ptr = ECS.mEntityMgr->Globals().mMapWindow->second.mComponents.GetComponent<pgn::cmp::cMapWindow>();
		lvl_ptr->mData.mLevelNode->attachTo(mwin_ptr->mData.mTileWin);
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionLevelLoad::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLoad>::Run");

		// TODO: Check if we have another level loaded, and unload if we do
		auto queryLvl = ECS.mSystemMgr->GetQuery("tag:CurrentLevel");

		// Get the tile layout & level from the input
		auto lvl_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cLevel>();
		auto& curlvl = lvl_ptr->mData;
		auto& mapdata = curlvl.mMapTileBg;

		// Mapwindow response
		auto e = ECS.mEntityMgr->Globals().mMapWindow;
		auto mwin_ptr = e->second.mComponents.GetComponent<pgn::cmp::cMapWindow>();
		auto & mwin = mwin_ptr->mData;

		mwin.mSprites.Resize(mapdata.Width(), mapdata.Height());

		// for each tile // TODO: use visitor instead? 
		for (unsigned i = 0; i < mapdata.Height(); ++i)
		{
			for (unsigned j = 0; j < mapdata.Width(); ++j)
			{
				// get entity from 2d map
				auto tile = curlvl.mDefaultTilesBg[ mapdata(j, i) ];

				// get sprite from entity, clone it and set data
				auto sprite_ptr = tile->second.mComponents.GetComponent<pgn::cmp::cMapSprite>();
				auto& winsprite = mwin.mSprites(j, i);
				winsprite = sprite_ptr->mData.mSprite->clone();
				winsprite->setSize(float(mwin.mTileDims.x), float(mwin.mTileDims.y));

				cmp::cLevelPosition pos;
				pos.mPos = glm::ivec2(j, i);
				winsprite->setPosition(mwin.LevelToScreenCoords(pos));
				winsprite->attachTo(lvl_ptr->mData.mLevelNode);
				winsprite->setPriority(short(sprite_ptr->mData.mRenderPriority));
			}
		}

		// For each entity, fix position
		for (const auto& x : curlvl.mMapTileRest.Store().mData)
		{
			auto pos = x.second->second.mComponents.GetComponent<cmp::cLevelPosition>();
			cActionLocationChange::RunEvent(x.second, ed, pos->mData.mPos);
		}
		
        // Put pc on a valid square
		auto epc = ECS.mEntityMgr->Globals().mPC;
		cActionLocationChange::RunEvent(epc, ed, curlvl.GetRandomStartPos());

		// Update level name in status window

		if (ECS.mEntityMgr->Globals().mStatusWindow != ECS.mEntityMgr->GetEntityData().end())
		{
			// Add to status info
			auto fstring_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cFormatString>();
			auto textwin_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cTextWindow>();
			fstring_ptr->mData.mDict["cln"] = ed->second.mName;
			auto text = fstring_ptr->mData.mText;
			EvalFormatString(fstring_ptr->mData.mDict, text);
			textwin_ptr->mData.SetText(text);
		}
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLevelUnload::Run(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelUnload>::Run");
		auto lvl_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cLevel>();
		lvl_ptr->mData.mLevelNode->detach();
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionLevelUnload::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelCreate>::Event");
		
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLevelEnter::Run(cEntityWithData ewho, cEntityWithData elvl)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelEnter>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionLevelEnter::Event(cEntityWithData ewho, cEntityWithData elvl)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelEnter>::Event");

		auto sprite_ptr = ewho->second.mComponents.GetComponent<pgn::cmp::cMapSprite>();
		auto pos_ptr = ewho->second.mComponents.GetComponent<pgn::cmp::cLevelPosition>();
		// set level to position entity
		pos_ptr->mData.mLevel = elvl;
		// add ewho to level entity list
		auto level_ptr = elvl->second.mComponents.GetComponent<pgn::cmp::cLevel>();
		level_ptr->mData.mEntities.insert(ewho);
		// attach to render scenegraph
		if (level_ptr->mData.mLevelNode != sprite_ptr->mData.mSprite->getParent())
			sprite_ptr->mData.mSprite->attachTo(level_ptr->mData.mLevelNode);
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLevelLeave::Run(cEntityWithData arg0, cEntityWithData arg1)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLeave>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionLevelLeave::Event(cEntityWithData ewho, cEntityWithData elvl)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LevelLeave>::Event");
		auto level_ptr = elvl->second.mComponents.GetComponent<pgn::cmp::cLevel>();
		level_ptr->mData.mEntities.erase(ewho);
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionExitApplication::Run()
	{
		cActionLog::RunEvent("system_log", "Called cAction<ExitApplication>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionExitApplication::Event()
	{
		cActionLog::RunEvent("system_log", "Called cAction<ExitApplication>::Run");
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLog::Run(const std::string& zLoggerName, const std::string& zString)
	{
		if (zLoggerName != "game_log") return true;
		auto queryLog = ECS.mSystemMgr->GetQuery("pgn::cmp::cLog");
		auto queryLogListener = ECS.mSystemMgr->GetQuery("tag:log:" + zLoggerName);
		bool ok = true;
		for (auto ec : queryLog->Entities())
		{
			// Move to the next logger if it's not the one we're looking for
			if (ec->second.mName != zLoggerName)
				continue;

			// Get the log
			auto log_ptr = ec->second.mComponents.GetComponent<pgn::cmp::cLog>();
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
				auto twin_ptr = ec->second.mComponents.GetComponent<pgn::cmp::cTextWindow>();
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
	void cActionLog::Event(const std::string& arg0, const std::string& arg1)
	{
		//assert(false);
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionActionIdle::Run(cEntityWithData arg0)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionIdle>::Run");
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionActionIdle::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionIdle>::Event");
		cActionLog::RunEvent("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[4]));
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionActionMoveAdj::Run(cEntityWithData ed, const glm::ivec2& vin)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionMoveAdj>::Run");

		glm::ivec2 v = vin;

		// Get entity data
		auto move_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cMovement>();
		auto pos_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cLevelPosition>();

        // Get layout of level
		auto level_ptr = pos_ptr->mData.mLevel->second.mComponents.GetComponent<pgn::cmp::cLevel>();
		const auto& mapdata = level_ptr->mData.mMapTileBg;
		
        // if the new position is within the bounds of the level
		const glm::ivec2 newpos = pos_ptr->mData.mPos + v;
		if (mapdata.InRange(newpos))
		{
            // Check if we hit an obstacle
			//auto obstacle_ptr = mapdata(newpos)->second.mComponents.GetComponent<pgn::cmp::cTileObstacle>();
			auto obstacle_ptr = level_ptr->mData.LookupTileBg(newpos.x,newpos.y)->second.mComponents.GetComponent<pgn::cmp::cTileObstacle>();
			if (!obstacle_ptr->mData.mIsObstacle)
			{
				cActionLocationChange::RunEvent(ed, pos_ptr->mData.mLevel, newpos);
				auto gamestats_ptr = ECS.mEntityMgr->Globals().mWorld->second.mComponents.GetComponent<pgn::cmp::cGameStats>();
				gamestats_ptr->mData.mTotalTurns[ed->first]++;

				// Update status text
				if (ed->first == ECS.mEntityMgr->Globals().mPC->first)
				{
					auto fstring_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cFormatString>();
					auto textwin_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cTextWindow>();
					fstring_ptr->mData.mDict["htt"] = std::to_string(gamestats_ptr->mData.mTotalTurns[ed->first]);
					auto text = fstring_ptr->mData.mText;
					EvalFormatString(fstring_ptr->mData.mDict, text);
					textwin_ptr->mData.SetText(text);
				}
				return true;
			}
		}
		return false;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionActionMoveAdj::Event(cEntityWithData ed, const glm::ivec2& v)
	{
		cActionLog::RunEvent("system_log", "Called cAction<ActionMoveAdj>::Event");

		// Log
		int udir = v.x + 1 + (v.y + 1) * 3;
		cActionLog::RunEvent("game_log", boost::str(boost::format("%s moved %s") % ed->second.mName.c_str() % dir2text_full[udir]));
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionDoorOpen::Run(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorOpen>::Run");

		auto door_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cDoor>();
		bool ok = door_ptr->mData.mIsClosed;
		if (ok)
			door_ptr->mData.mIsClosed = false;
		return ok;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionDoorOpen::Event(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorOpen>::Event");

		auto door_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cDoor>();
		auto sprite_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cMapSprite>();
		sprite_ptr->mData.mSprite->setAnimFrame(door_ptr->mData.mSprites[0].mSprite->getAnimFrame());
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionDoorClose::Run(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorClose>::Run");

		auto door_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cDoor>();
		bool ok = !door_ptr->mData.mIsClosed;
		if (ok)
			door_ptr->mData.mIsClosed = true;
		return ok;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionDoorClose::Event(cEntityWithData ewho, cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<DoorClose>::Event");
		auto door_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cDoor>();
		auto sprite_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cMapSprite>();
		sprite_ptr->mData.mSprite->setAnimFrame(door_ptr->mData.mSprites[1].mSprite->getAnimFrame());
	}


	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionKeyState::Run(const int key, const oxygine::cKeyState& state)
	{
		cActionLog::RunEvent("system_log", "Called cAction<KeyState>::Run");

		// Debug key presses
		//std::cout << boost::str(boost::format("%s - %s - %d\n")% (state.mPressed ? "pressed" : "released")%(state.mRepeat ? "repeat" : "no-repeat")%key );

		auto queryKam = ECS.mSystemMgr->GetQuery("pgn::cmp::cKeyActionMapper");

		for (auto ec : queryKam->Entities())
		{
			// Get the action mapper
			auto kam_ptr = ec->second.mComponents.GetComponent<pgn::cmp::cKeyActionMapper>();
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
	void cActionKeyState::Event(const int, const oxygine::cKeyState&)
	{
		cActionLog::RunEvent("system_log", "Called cAction<KeyState>::Event");
		//assert(false);
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionEntityCreated::Run(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<EntityCreated>::Run");
		
		// Is it a door?
		auto door_ptr = ed->second.mComponents.GetComponent<cmp::cDoor>();
		if (door_ptr)
		{
			// set the initial state
			if (door_ptr->mData.mIsClosed)
				cActionDoorClose::Event(ed,ed);
			else
				cActionDoorOpen::Event(ed,ed);
		}
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionEntityCreated::Event(cEntityWithData ed)
	{
		cActionLog::RunEvent("system_log", "Called cAction<EntityCreated>::Event");		
		
		// Set render priority dep. on type
		auto sprite_ptr = ed->second.mComponents.GetComponent<pgn::cmp::cMapSprite>();
		if (sprite_ptr)
		{
			if (ed->second.mTags.find("type:tile_bg") != ed->second.mTags.end())
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::DungeonElementBG;
			else if (ed->second.mTags.find("type:tile_fg") != ed->second.mTags.end())
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::DungeonElementFG;
			else if (ed->second.mTags.find("type:item") != ed->second.mTags.end())
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::Item;
			else if (ed->second.mTags.find("type:creature") != ed->second.mTags.end())
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::Creature;
			else
				sprite_ptr->mData.mRenderPriority = eMapRenderOrder::DungeonElementBG;
			sprite_ptr->mData.mSprite->setPriority(to_integral(sprite_ptr->mData.mRenderPriority));
		}

		if (ECS.mEntityMgr->Globals().mStatusWindow != ECS.mEntityMgr->GetEntityData().end())
		{
			// Add to status info
			auto fstring_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cFormatString>();
			auto textwin_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cTextWindow>();
			fstring_ptr->mData.mDict["tee"] = std::to_string(ECS.mEntityMgr->GetEntityData().size());
			auto text = fstring_ptr->mData.mText;
			EvalFormatString(fstring_ptr->mData.mDict, text);
			textwin_ptr->mData.SetText(text);
		}
	}

	//####################################################################################################
	//----------------------------------------------------------------------------------------------------
	template<>
	bool cActionLocationChange::Run(cEntityWithData ewho, cEntityWithData elvl, const glm::ivec2& pos)
	{	
		auto pos_ptr = ewho->second.mComponents.GetComponent<pgn::cmp::cLevelPosition>();
		pos_ptr->mData.mPos = pos;
		return true;
	}


	//----------------------------------------------------------------------------------------------------
	template<>
	void cActionLocationChange::Event(cEntityWithData ewho, cEntityWithData elvl, const glm::ivec2& pos)
	{
		cActionLog::RunEvent("system_log", "Called cAction<LocationChange>::Run");

		auto sprite_ptr = ewho->second.mComponents.GetComponent<pgn::cmp::cMapSprite>();
		auto pos_ptr = ewho->second.mComponents.GetComponent<pgn::cmp::cLevelPosition>();
		auto e = ECS.mEntityMgr->Globals().mMapWindow;
		auto mwin_ptr = e->second.mComponents.GetComponent<pgn::cmp::cMapWindow>();
		auto & mwin = mwin_ptr->mData;
		sprite_ptr->mData.mSprite->setPosition(mwin.LevelToScreenCoords(pos_ptr->mData));

		// Attach to level
		if (pos_ptr->mData.mLevel != elvl)
		{
			if (pos_ptr->mData.mLevel != ECS.mEntityMgr->GetEntityData().end())
				cActionLevelLeave::RunEvent(ewho, pos_ptr->mData.mLevel);
			cActionLevelEnter::RunEvent(ewho, elvl);
		}

		// Update position in status window
		if (ewho->first == ECS.mEntityMgr->Globals().mPC->first)
		{
			auto fstring_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cFormatString>();
			auto textwin_ptr = ECS.mEntityMgr->Globals().mStatusWindow->second.mComponents.GetComponent<pgn::cmp::cTextWindow>();
			fstring_ptr->mData.mDict["cpp"] = glm::to_string( pos);
			auto text = fstring_ptr->mData.mText;
			EvalFormatString(fstring_ptr->mData.mDict, text);
			textwin_ptr->mData.SetText(text);
		}
	}

}