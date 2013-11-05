#include "AsciiStatusRenderSystem.h"

#include <curses.h>

#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/AsciiWindow.h>

namespace pgn
{
	void cAsciiStatusRenderSystem::Process()
	{
		std::shared_ptr< cComponent<cAsciiWindow>> asciiwin_ptr;
		for (auto e : mQuery->Entities())
		{
			auto ec = ECS.mEntityMgr->GetComponents().find(e);
			assert(ec != ECS.mEntityMgr->GetComponents().end());

			ec->second.GetComponent(asciiwin_ptr);
		}
	}

	bool cAsciiStatusRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		cSystemBase::from_json(zRoot);
		
		mQuery = std::shared_ptr< cQueryExpression>(new cQueryExpression());
		if (pgn::from_json(*mQuery, zRoot["Query"]))
			ECS.mSystemMgr->AddQuery(to_string(mQuery->Hash()), mQuery);

		return mQuery != nullptr;
	}
}