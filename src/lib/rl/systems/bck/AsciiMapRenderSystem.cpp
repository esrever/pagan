#include "AsciiMapRenderSystem.h"

#include <curses.h>

#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/AsciiWindow.h>
#include <rl/components/Level.h>
#include <rl/components/AsciiRep.h>

namespace pgn
{
	void cAsciiMapRenderSystem::Process()
	{
	
		assert(mQueryWin->Entities().size() == 1);
		//assert(mQueryLvl->Entities().size() == 1);

		std::shared_ptr< cComponent<cAsciiWindow>> asciiwin_ptr;
		auto ec = ECS.mEntityMgr->GetComponents().find( *mQueryWin->Entities().begin());
		assert(ec != ECS.mEntityMgr->GetComponents().end());
		ec->second.GetComponent(asciiwin_ptr);

		std::shared_ptr< cComponent<cLevel>> level_ptr;
		std::shared_ptr< cComponent<cAsciiRep>> char_ptr;
		for (auto e : mQueryLvl->Entities())
		{
			auto ec = ECS.mEntityMgr->GetComponents().find(e);
			assert(ec != ECS.mEntityMgr->GetComponents().end());
			ec->second.GetComponent(level_ptr);
		}

		for (auto e : mQueryChar->Entities())
		{
			auto ec = ECS.mEntityMgr->GetComponents().find(e);
			assert(ec != ECS.mEntityMgr->GetComponents().end());
			ec->second.GetComponent(char_ptr);
		}

		auto win = *asciiwin_ptr->mData.mWindow.get();
		wprintw(win, "Map render");
		wrefresh(win);
	}

	bool cAsciiMapRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		cSystemBase::from_json(zRoot);

		mQueryWin = std::shared_ptr< cQueryExpression>(new cQueryExpression());
		if (pgn::from_json(*mQueryWin, zRoot["QueryWin"]))
			ECS.mSystemMgr->AddQuery(to_string(mQueryWin->Hash()), mQueryWin);

		mQueryLvl = std::shared_ptr< cQueryExpression>(new cQueryExpression());
		if (pgn::from_json(*mQueryLvl, zRoot["QueryLvl"]))
			ECS.mSystemMgr->AddQuery(to_string(mQueryLvl->Hash()), mQueryLvl);

		mQueryChar = std::shared_ptr< cQueryExpression>(new cQueryExpression());
		if (pgn::from_json(*mQueryChar, zRoot["QueryChar"]))
			ECS.mSystemMgr->AddQuery(to_string(mQueryChar->Hash()), mQueryChar);

		return mQueryChar && mQueryLvl && mQueryWin;
	}

	void cAsciiMapRenderSystem::to_json(JsonWriter& zRoot) const
	{
		zRoot.StartObject();
		zRoot.String("Base");
		cSystemBase::to_json(zRoot);
		JsonWriter_AddMember("QueryWin", mQueryWin, zRoot);
		JsonWriter_AddMember("QueryLvl", mQueryLvl, zRoot);
		JsonWriter_AddMember("QueryChar", mQueryChar, zRoot);
		zRoot.EndObject();
	}
}