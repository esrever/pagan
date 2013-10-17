#pragma once

#include "ActionMappingSystem.h"

#include <curses.h>

#include <ecs/ecs.h>
#include <rl/events/events.h>

namespace pgn
{
	//------------------------------------------------------
	void cActionMappingSystem::Process()
	{
		// TODO: the listener will check 1) if it's active 2) against a map of keys to:
		//		 void function(), or a signal
		chtype ch = getch();
		glm::ivec2 gotoDir(0,0);
		switch(ch)
		{
			case 27: 
				cExitApplicationEventData::emit(); 
				break;
			case '1':
			case KEY_END:
			case KEY_C1:
				gotoDir.x--;gotoDir.y--; break;
			case '2': 
			case KEY_DOWN:
			case KEY_C2:
				gotoDir.x  ;gotoDir.y--; break;
			case '3': 
			case KEY_NPAGE:
			case KEY_C3:
				gotoDir.x++;gotoDir.y--; break;
			case '7': 
			case KEY_HOME:
			case KEY_A1:
				gotoDir.x--;gotoDir.y++; break;
			case '8': 
			case KEY_UP:
			case KEY_A2:
				gotoDir.x  ;gotoDir.y++; break;
			case '9': 
			case KEY_PPAGE:
			case KEY_A3:
				gotoDir.x++;gotoDir.y++; break;
			case '4':
			case KEY_LEFT:
			case KEY_B1:
				gotoDir.x--;gotoDir.y  ; break;
			case '5': 
			case KEY_B2:
				gotoDir.x  ;gotoDir.y  ; break;
			case '6': 
			case KEY_RIGHT:
			case KEY_B3:
				gotoDir.x++;gotoDir.y  ; break;
			default: break;
		};
		ECS.mLog.Dbg(boost::str(boost::format("Pressed key '%d'")%ch));
		if(ch == 27) cExitApplicationEventData::emit();
	}

	//------------------------------------------------------
	void cActionMappingSystem::from_json(const rapidjson::Value& zRoot)
	{
		cSystemBase::from_json(zRoot);
	}
}