#include "SystemMgr.h"
#include "ecs.h"
#include "EntityMgr.h"
#include "SystemBase.h"
#include "ComponentQuery.h"

namespace pgn
{
	void cSystemMgr::from_json(const rapidjson::Value& zRoot)
	{
		const auto&  qobj = zRoot["Queries"];
		if(qobj.IsObject())
			for (auto itr = qobj.MemberBegin(); itr != qobj.MemberEnd(); ++itr) 
			{
				// get a query object
				const auto& qcur = *itr;
				const auto& qname = qcur.name;
				const auto& types = qcur.value;
				component_mask_type mask;
				bool err = false;
				assert(types.IsArray());
				for (auto itr2 = types.Begin(); itr2 != types.End(); ++itr2) 
				{
					const auto& s = itr2->GetString();
					auto idx = EMGR->GetComponentTypeIndex(s);
					if(idx != 0xFFFFFFFF)
						mask.set(idx);
					else
					{
						ECS.mLog.Err(boost::str(boost::format("cSystemMgr::from_json: type \"%s\" does not exist")%s));
						err = true;
						break;
					}; 
				}
				if(!err)
					mComponentQueries.insert( std::pair<std::string, cComponentQuery>( qname.GetString(),cComponentQuery(mask)));
			}
		else
			ECS.mLog.Wrn("cSystemMgr::from_json: \"Queries\" not found");
		const auto&  sobj = zRoot["Systems"];
		if(sobj.IsArray())
			for (auto itr = sobj.Begin(); itr != sobj.End(); ++itr) 
			{
				// get a system object
				const auto& scur = *itr;
				assert(scur.IsObject());

				// Get type
				assert(scur.HasMember("Type"));
				const auto& stype = scur["Type"].GetString();

				// Get priority
				const size_t priority = scur.HasMember("Priority") ? scur["Priority"].GetUint() : 0xFFFFFFFF;
				const auto& sdata = scur.value;
				
				// Read Type
				assert(sdata.HasMember("Type"));
				const auto& stype = sdata["Type"].GetString();
				// Read Desc
				const auto desc = sdata.HasMember("Desc") ? sdata["Desc"].GetString() : "";
				// Read Priority
				
				// Read Used queries
				assert(sdata.HasMember("QueriesUsed"));
				const auto& qused = sdata["QueriesUsed"];
				assert(qused.IsArray());
				std::vector<std::string> str_qused;
				for (auto itr2 = qused.Begin(); itr2 != qused.End(); ++itr2) 
					str_qused.push_back(itr2->GetString());
				// Read init data
				assert(sdata.HasMember("Init"));
				const auto& initdata = sdata["Init"];
				// Create the system
				auto sptr = Create(sname, stype, desc, str_qused, initdata);
				if(sptr)
					mSystems.insert( std::pair<size_t, std::shared_ptr<cSystemBase>>(priority, sptr));
			}
		else
			ECS.mLog.Wrn("cSystemMgr::from_json: \"Systems\" array not found");
	}

	//-------------------------------------------------------------------------------------------------
	std::shared_ptr<cSystemBase>  cSystemMgr::Create(const std::string& zName,
													const std::string& zType,
													const std::string& zDesc,
													const std::vector<std::string>& zQueriesUsed,
													const rapidjson::Value& zInitData)
	{
		return nullptr;
	}
}