#include "SystemBase.h"

#include <boost/format.hpp>

#include "ecs.h"
#include "Event.h"
#include "QueryBase.h"

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
		cSystemActivityEventData::emit(std::make_tuple(zActive));
	}

	//---------------------------------------------------------------
	void cSystemBase::ProcessQuery(const std::string& zQueryName)
	{
		// Get the query. This should be easily retrievable
		auto qit = ECS.mSystemMgr->GetQueries().find(zQueryName);
		if( qit != ECS.mSystemMgr->GetQueries().end())
		{
			// Get entities found by the query
			auto ents = qit->second.Get();
			for(auto e : ents)
			{
				// Get the entities' components
				auto ec = ECS.mEntityMgr->GetComponents().find(e);
				ProcessSingle(ec);
			}
		}
	}

	//---------------------------------------------------------------
	bool cSystemBase::from_json(const rapidjson::Value& zRoot)
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
				mReferencedQueries.push_back( std::make_shared<cQueryBase>(it->second));
			else
				ECS.mLog.Err(boost::str(boost::format("cSystemBase::from_json: Failed to find query \"%s\"")%s));;
		}
		return true;
	}

	//---------------------------------------------------------------
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, rapidjson::Value& zRoot) 
	{
		zSys.to_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	bool from_json<cSystemBase>(cSystemBase& zSys, const rapidjson::Value& zRoot) 
	{
		return zSys.from_json(zRoot);
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