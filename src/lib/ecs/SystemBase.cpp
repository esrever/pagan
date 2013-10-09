#include "SystemBase.h"
#include "ecs.h"
#include "Event.h"

namespace pgn
{
	//---------------------------------------------------------------
	cSystemBase::cSystemBase()
		:mActive(false)
	{}

	//---------------------------------------------------------------
	cSystemBase::~cSystemBase()
	{
		SetActive(false);
	}
	
	//---------------------------------------------------------------
	void cSystemBase::SetActive(bool zActive)
	{
		cSystemActivityEventData::emit(zActive);
	}

	//---------------------------------------------------------------
	void cSystemBase::from_json(const rapidjson::Value& zRoot)
	{
		// Read Desc
		const auto desc = zRoot.HasMember("Desc") ? zRoot["Desc"].GetString() : "";
		// Read Used queries
		assert(zRoot.HasMember("QueriesUsed"));
		const auto& qused = zRoot["QueriesUsed"];
		assert(qused.IsArray());
		const auto& allQueries = ECS.mSystemMgr->GetQueries();
		for (auto itr2 = qused.Begin(); itr2 != qused.End(); ++itr2) 
		{
			const auto& s = itr2->GetString();
			auto it = allQueries.find(s);
			if( it != allQueries.end())
				mReferencedQueries.push_back(it);
			else
				;
		}
	}

	//---------------------------------------------------------------
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, rapidjson::Value& zRoot) 
	{
		zSys.to_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	void from_json<cSystemBase>(cSystemBase& zSys, const rapidjson::Value& zRoot) 
	{
		zSys.from_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	std::string to_string<cSystemBase>(const cSystemBase& zSys) 
	{
		rapidjson::Value root;
		zSys.to_json(root);
		return to_string(root);
	}
}