#include "ecs.h"

namespace pgn
{
	const std::string cECS::mErrorString("ECS_Error");

	void cECS::Init()
	{
		std::ios_base::Init();
		mLog.SetName("ECS");
		mLog.SetChannel(&std::cout);
		mDataPath = __FILE__;
		mDataPath.erase( mDataPath.end()-7, mDataPath.end());
		mDataPath.append( "../rl/data/");
	}

	void cECS::Destroy()
	{
		mSystemMgr.reset();
		mEntityMgr.reset();
	}

	bool cECS::from_json(const rapidjson::Value& zRoot)
	{
		const auto& ment = zRoot["EntityMgr"];
		bool r1 = mEntityMgr->from_json(ment);
		const auto& msys = zRoot["SystemMgr"];
		bool r2 = mSystemMgr->from_json(msys);
        return r1 && r2;
	}
}