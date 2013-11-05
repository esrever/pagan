#include "AsciiMapRenderSystem.h"

#include <curses.h>

#include <ecs/Component.h>
#include <ecs/EntityComponents.h>

#include <rl/components/AsciiWindow.h>

namespace pgn
{
	void cAsciiMapRenderSystem::Process()
	{
		
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
}