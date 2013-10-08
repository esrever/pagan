#include "SystemMgr.h"
#include "ecs.h"
#include "EntityMgr.h"
#include "SystemBase.h"
#include "ComponentQuery.h"

namespace pgn
{
	//--------------------------------------------------------------------------------------
	void cSystemMgr::AddSystem(std::shared_ptr<cSystemBase> zSystem, int zPriority)
	{
		//mSystems.insert( std::pair<>)
	}

	//--------------------------------------------------------------------------------------
	void cSystemMgr::RemoveSystem(std::shared_ptr<cSystemBase> zSystem)
	{

	}

	//--------------------------------------------------------------------------------------
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
						ECS.mLog.Err(boost::str(boost::format("cSystemMgr::from_json: Component type \"%s\" does not exist")%s));
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
		if(sobj.IsObject())
			for (auto itr = sobj.MemberBegin(); itr != sobj.MemberEnd(); ++itr) 
			{
				// get a system object
				const auto& scur = *itr;
				const auto& sname = scur.name.GetString();
				const auto& sdata = scur.value;
				assert(sdata.IsObject());
				// Read Type
				assert(sdata.HasMember("Type"));
				const auto& stype = sdata["Type"].GetString();
				// Read Desc
				const auto desc = sdata.HasMember("Desc") ? sdata["Desc"].GetString() : "";
				// Read Priority
				const size_t priority = sdata.HasMember("Priority") ? sdata["Priority"].GetUint() : 0xFFFFFFFF;
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
				// TODO: use/rearrange sname, desc, str_qused, initdata
				auto sptr = Create(stype);
				if(sptr)
					AddSystem(sptr, priority);
				else
					ECS.mLog.Err(boost::str(boost::format("cSystemMgr::from_json: Failed to create system of type \"%s\"")%stype));
			}
		else
			ECS.mLog.Wrn("cSystemMgr::from_json: \"Systems\" not found");
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
}