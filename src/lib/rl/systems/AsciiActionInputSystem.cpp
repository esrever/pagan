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
		
		for (auto e : mQuery->Entities())
		{
			auto ec = ECS.mEntityMgr->GetComponents().find(e);
			assert(ec != ECS.mEntityMgr->GetComponents().end());

			// Get keyactionmapper component
			std::shared_ptr< cComponent<cKeyActionMapper>> ptr;
			ec->second.GetComponent(ptr);

			// find function
			auto itFunc = ptr->mData.mActions.find(mCurChar);
			if (itFunc != ptr->mData.mActions.end())
			{
				// if found, execute
				itFunc->second(ec->first);
			}
		}

	}

	//------------------------------------------------------
	bool cAsciiActionInputSystem::from_json(const rapidjson::Value& zRoot)
	{
		cSystemBase::from_json(zRoot);

		mQuery = std::shared_ptr< cQueryExpression>( new cQueryExpression());
		if (pgn::from_json(*mQuery, zRoot["Query"]))
			ECS.mSystemMgr->AddQuery(to_string(mQuery->Hash()), mQuery);
		
		return mQuery != nullptr;
	}

	void cAsciiActionInputSystem::to_json(JsonWriter& zRoot) const
	{
		zRoot.StartObject();
		zRoot.String("Base");
		cSystemBase::to_json(zRoot);
		JsonWriter_AddMember("Query", mQuery,zRoot);
		zRoot.EndObject();
	}
}