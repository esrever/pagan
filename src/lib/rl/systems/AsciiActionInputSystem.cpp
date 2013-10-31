#pragma once

#include "AsciiActionInputSystem.h"

#include <curses.h>

#include <ecs/ecs.h>
#include <ecs/SystemMgr.h>
#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/KeyActionMapper.h>

namespace pgn
{
	//------------------------------------------------------
	void cAsciiActionInputSystem::Process()
	{
		mCurChar = static_cast<wchar_t>(getch());
		/*
			Summary of below process:
			get query
			get query's entities
			for each entity
				get components
				get keyactionmapper component
				search for function
				execut function

		*/
		
		ProcessQuery("KeyInputListener");
	}

	void cAsciiActionInputSystem::ProcessSingle(const std::map< cEntity, cEntityComponents>::const_iterator& zEc)
	{
		// Get keyactionmapper component
		std::shared_ptr< cComponent<cKeyActionMapper>> ptr;
		zEc->second.GetComponent( ptr);
				
		// find function
		auto itFunc = ptr->mData.mActions.find(mCurChar);
		if(itFunc != ptr->mData.mActions.end() )
		{
			// if found, execute
			itFunc->second(zEc->first);
		}
	}

	//------------------------------------------------------
	bool cAsciiActionInputSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}