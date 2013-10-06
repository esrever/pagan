#include "ecs.h"

namespace pgn
{
	void cECS::Init()
	{
		std::ios_base::Init();
		mLog.SetName("ECS");
		mLog.SetChannel(&std::cout);
		// TODO: from_json. Add project source to path or config file
	}

	void cECS::from_json(const rapidjson::Value& zRoot)
	{
		const auto& ment = zRoot["EntityMgr"];
		mEntityMgr.from_json(ment);
		const auto& msys = zRoot["SystemMgr"];
		mSystemMgr.from_json(msys);
	}
}