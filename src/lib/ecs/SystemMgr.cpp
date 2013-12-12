#include "SystemMgr.h"

#include <core/util/predicates.h>
#include <core/util/jsonutils.h>

#include "ecs.h"
#include "EntityMgr.h"
#include "SystemBase.h"
#include "EventHandlerQuery.h"

namespace pgn
{
	//--------------------------------------------------------------------------------------
	void cSystemMgr::AddSystem(std::shared_ptr<cSystemBase> zSystem, int zPriority)
	{
		mSystems.insert( std::pair<size_t, std::shared_ptr<cSystemBase>>(zPriority,zSystem));
	}

	//--------------------------------------------------------------------------------------
	void cSystemMgr::RemoveSystem(std::shared_ptr<cSystemBase> zSystem)
	{

	}

	//--------------------------------------------------------------------------------------
	bool cSystemMgr::from_json(const rapidjson::Value& zRoot)
	{
		RegisterAllEvents();

		std::vector<std::string> fnames;
		read_json_vector(fnames, zRoot["QueriesFile"]);
		for(auto s : fnames)
		{
			auto pdoc = file_to_json(ECS.GetDataPath() + s);
			ImportQueries(pdoc.get());
		}
		/*
		read_json_vector(fnames, zRoot["SystemsFile"]);
		for(auto s : fnames)
		{
			auto pdoc = file_to_json(ECS.GetDataPath() + s);
			ImportSystems(pdoc.get());
		}
		*/
		return true;
	}

	//-------------------------------------------------------------------------------------------------
	std::shared_ptr<cSystemBase> cSystemMgr::Create(const std::string& zName) const
	{
		auto it = mSystemCreators.find(zName);
		if(it == mSystemCreators.end())
			return nullptr;
		else
			return it->second();
	}

	//----------------------------------------------------------------
	void cSystemMgr::ImportSystems(const rapidjson::Document * zDoc)
	{
		if (!zDoc) return;
		if(zDoc->IsArray())
			for (auto itr = zDoc->Begin(); itr != zDoc->End(); ++itr) 
			{
				// get a system object
				const auto& scur = *itr;
				assert(scur.IsObject());

				// Get type
				assert(scur.HasMember("Type"));
				const auto& stype = scur["Type"].GetString();

				// Get priority
				const size_t priority = scur.HasMember("Priority") ? scur["Priority"].GetUint() : 0xFFFFFFFF;
		
				auto sptr = Create(stype);
				if(sptr)
				{
					assert(scur.HasMember("Data"));
					const auto& sdata = scur["Data"];
					::pgn::from_json(*sptr, sdata);
					AddSystem(sptr, priority);
				}
				else
					ECS.mLog->Err(boost::str(boost::format("cSystemMgr::from_json: Failed to create system of type \"%s\"")%stype));
			}
		else
			ECS.mLog->Wrn("cSystemMgr::from_json: \"Systems\" array not found");
	}

	//----------------------------------------------------------------
	void cSystemMgr::ImportQueries(const rapidjson::Document * zDoc)
	{
		if (!zDoc) return;
		if (zDoc->IsObject())
		{
			for (auto itr = zDoc->MemberBegin(); itr != zDoc->MemberEnd(); ++itr)
			{
				const auto& qcur = *itr;
				const auto& qname = qcur.name;
				const auto& qdata = qcur.value;
				auto qexp = std::shared_ptr<cQueryExpression>(new cQueryExpression());
				if ( pgn::from_json(*qexp, qdata))
					mQueries.insert(std::pair<std::string, cQueryExpressionSptr>(qname.GetString(), qexp));
			}
		}
	}

	//----------------------------------------------------------------
	void cSystemMgr::AddQuery(const std::string& zName, const cQueryExpressionSptr& zPtr)
	{
		mQueries[zName] = zPtr;
	}

	//----------------------------------------------------------------
	cQueryExpressionSptr cSystemMgr::GetQuery(const std::string& zName)
	{
		auto it = mQueries.find(zName);
		if (it == mQueries.end())
		{
			auto qexp = std::shared_ptr<cQueryExpression>(new cQueryExpression());
			qexp->AddString(zName);
			AddQuery(zName, qexp);
			return qexp;
		}
		else
			return it->second;
	}

	//----------------------------------------------------------------
	template<>
	void to_json<cSystemMgr>(const cSystemMgr& zMgr, JsonWriter& writer)
	{
		writer.StartObject();

		// Archetypes
		JsonWriter_AddMember("Queries", zMgr.mQueries, writer);
		JsonWriter_AddMember("Systems", zMgr.mSystems, writer);

		writer.EndObject();
	}
}