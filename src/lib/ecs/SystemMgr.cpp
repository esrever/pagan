#include "SystemMgr.h"

#include <core/util/predicates.h>

#include "ecs.h"
#include "EntityMgr.h"
#include "SystemBase.h"

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
		
				auto sptr = Create(stype);
				if(sptr)
				{
					assert(scur.HasMember("Data"));
					const auto& sdata = scur["Data"];
					sptr->from_json(sdata);
					AddSystem(sptr, priority);
				}
				else
					ECS.mLog.Err(boost::str(boost::format("cSystemMgr::from_json: Failed to create system of type \"%s\"")%stype));
			}
		else
			ECS.mLog.Wrn("cSystemMgr::from_json: \"Systems\" array not found");
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

	//--------------------------------------------------------------------------------------------------
	const std::string& cSystemMgr::GetQueryName(const cComponentQuery& zQuery) const
	{
		const auto& it = std::find_if(mComponentQueries.begin(), mComponentQueries.end(), cMapValueFinder<std::string,cComponentQuery>(zQuery));
		if(it == mComponentQueries.end())
			return ECS.mErrorString;
		else
			return it->first;
	}
}