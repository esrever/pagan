#pragma once

#include "MapInputSystem.h"

#include <curses.h>

#include <ecs/ecs.h>
#include <ecs/ComponentHelpers.h>
#include <rl/events/events.h>

namespace pgn
{
	//------------------------------------------------------
	void cMapInputSystem::Process()
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
		auto & ecs = ECS;		
		ecs.mLog.Dbg(boost::str(boost::format("Pressed key '%d'")%ch));
		if(ch == 27) cExitApplicationEventData::emit();

		auto it = ecs.mEntityMgr->TaggedEntities().find("player");
		if( it != ecs.mEntityMgr->TaggedEntities().end() )
		{
			// get entity set
			const auto& it2 = it->second;
			assert(it2.size() == 1); // Only 1 player!
			// get entity
			const auto& e = *it2.begin();
			// get entity components
			const auto& compos = ecs.mEntityMgr->GetComponents(e);

			std::shared_ptr<cComponent<cLocation>> ploc;
			compos.GetComponent(ploc);

			/*
			// get location component
			const auto& itcompo = compos.Components().at( cComponent<cLocation>::StaticTypeIndex());

			// pointer to location - TODO: have a better interface to be getting the typed components directly. Impl in EntityComponents
			auto pLoc = static_cast<cComponent<cLocation> *>(itcompo.get());

			std::shared_ptr<cComponent<cLocation>> ptr;
			GetComponent<cLocation>(e, ptr);
			*/

			//TODO: update location with the got dir
			//		Send MoveToAdjacent event, which does the rest of the logic

			//TODO: helper functions to get player character, specific components etc. observe usage patterns
		}
	}


	//------------------------------------------------------
	bool cMapInputSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}