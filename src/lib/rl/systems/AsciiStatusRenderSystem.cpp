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
		for (auto e : mQueryWin->Entities())
		{
			auto ec = ECS.mEntityMgr->GetComponents().find(e);
			assert(ec != ECS.mEntityMgr->GetComponents().end());

			ec->second.GetComponent(asciiwin_ptr);
		}

		auto win = *asciiwin_ptr->mData.mWindow.get();
		wprintw(win, "Status render");
		wrefresh(win);
	}

	bool cAsciiStatusRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		cSystemBase::from_json(zRoot);
		
		auto b0 = LoadQuery(mQueryWin, zRoot,"QueryWin");
		auto b1 = LoadQuery(mQueryStatus, zRoot, "QueryStatus");
		return b0 && b1;
	}

	void cAsciiStatusRenderSystem::to_json(JsonWriter& zRoot) const
	{
		zRoot.StartObject();
		zRoot.String("Base");
		cSystemBase::to_json(zRoot);
		JsonWriter_AddMember("QueryWin", mQueryWin, zRoot);
		JsonWriter_AddMember("QueryStatus", mQueryStatus, zRoot);
		zRoot.EndObject();
	}
}