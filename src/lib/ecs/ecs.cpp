#include "ecs.h"

namespace pgn
{
	void cECS::Init()
	{
		std::ios_base::Init();
		mLog.SetName("ECS");
		mLog.SetChannel(&std::cout);
	}

	void cECS::Destroy()
	{
		mSystemMgr.reset();
		mEntityMgr.reset();
	}

	void cECS::from_json(const rapidjson::Value& zRoot)
	{
		const auto& ment = zRoot["EntityMgr"];
		mEntityMgr->from_json(ment);
		const auto& msys = zRoot["SystemMgr"];
		mSystemMgr->from_json(msys);
	}
}