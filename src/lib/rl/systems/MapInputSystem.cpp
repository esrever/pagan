#pragma once

#include "MapInputSystem.h"

#include <curses.h>

#include <ecs/ecs.h>
#include <ecs/ComponentHelpers.h>
#include <rl/events/events.h>

#include <boost/format.hpp>

namespace pgn
{
	//------------------------------------------------------
	void cMapInputSystem::Process()
	{
		// TODO: the listener will check 1) if it's active 2) against a map of keys to:
		//		 void function(), or a signal
		chtype ch = getch();
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