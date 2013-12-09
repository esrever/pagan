#include "ecs.h"

namespace pgn
{
	const std::string cECS::mErrorString("ECS_Error");

	void cECS::Init()
	{
		std::ios_base::Init();
		auto log_ptr = std::make_shared<pgn::cLogStream>(pgn::cLogStream());
		log_ptr->SetChannel(&std::cout);
		mLog = std::dynamic_pointer_cast<pgn::cLogBase>(log_ptr);
		cLogStream::Default() = *log_ptr;
		mLog->SetName("ECS");
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

	template<>
	void to_json(const cECS& zObj, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		JsonWriter_AddMember("EntityMgr", zObj.mEntityMgr, zRoot);
		JsonWriter_AddMember("SystemMgr", zObj.mSystemMgr, zRoot);
		zRoot.EndObject();
	}
}