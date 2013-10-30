#pragma once

#include "AsciiActionInputSystem.h"

#include <curses.h>

#include <ecs/ecs.h>
#include <ecs/SystemMgr.h>

#include <rl/components/KeyActionMapper.h>

namespace pgn
{
	//------------------------------------------------------
	void cAsciiActionInputSystem::Process()
	{
		auto ch = static_cast<wchar_t>(getch());
		// Get the query. This should be easily retrievable
		auto qit = ECS.mSystemMgr->GetQueries().find("InputListener");
		if( qit != ECS.mSystemMgr->GetQueries().end())
		{
			// Get entities found by the query
			auto ents = qit->second.Get();
			for(auto e : ents)
			{
				// Get the entities' components
				const auto& ec = ECS.mEntityMgr->GetComponents(e);

				// Get keyactionmapper component
				std::shared_ptr< cComponent<cKeyActionMapper>> ptr;
				ec.GetComponent( ptr);
				
				// find function
				auto itFunc = ptr->mData.mActions.find(ch);
				if(itFunc != ptr->mData.mActions.end() )
				{
					// if found, execute
					itFunc->second(e);
				}
			}
		}
	}

	//------------------------------------------------------
	bool cAsciiActionInputSystem::from_json(const rapidjson::Value& zRoot)
	{
		return cSystemBase::from_json(zRoot);
	}
}