#include "SystemMgr.h"

#include <core/util/predicates.h>
#include <core/util/jsonutils.h>

#include "ecs.h"
#include "EntityMgr.h"
#include "SystemBase.h"

#include "ComponentQuery.h"
#include "TagQuery.h"

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
		std::vector<std::string> fnames;
		read_json_vector(fnames, zRoot["QueriesFile"]);
		for(auto s : fnames)
		{
			auto pdoc = file_to_json(ECS.GetDataPath() + s);
			ImportQueries(pdoc.get());
		}
		read_json_vector(fnames, zRoot["SystemsFile"]);
		for(auto s : fnames)
		{
			auto pdoc = file_to_json(ECS.GetDataPath() + s);
			ImportSystems(pdoc.get());
		}
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

	//--------------------------------------------------------------------------------------------------
	const std::string& cSystemMgr::GetQueryName(const cQueryBase& zQuery) const
	{
		const auto& it = std::find_if(mQueries.begin(), mQueries.end(), cMapValueFinder<std::string,cQueryBase>(zQuery));
		if(it == mQueries.end())
			return ECS.GetErrorString();
		else
			return it->first;
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
					sptr->from_json(sdata);
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
		if(zDoc->IsObject())
		{
			const auto& zDocRef = *zDoc;
			const auto& zDocCompo = zDocRef["Component"];
			if(zDocCompo.IsObject())
				for (auto itr = zDocCompo.MemberBegin(); itr != zDocCompo.MemberEnd(); ++itr) 
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
							ECS.mLog->Err(boost::str(boost::format("cSystemMgr::from_json: Component type \"%s\" does not exist")%s));
							err = true;
							break;
						}; 
					}
					if(!err)
						mQueries.insert( std::pair<std::string, cQueryBase>( qname.GetString(),cComponentQuery(mask)));
				}
			const auto& zDocTag = zDocRef["Tag"];
				for (auto itr = zDocTag.MemberBegin(); itr != zDocTag.MemberEnd(); ++itr) 
				{
					// get a query object
					const auto& qcur = *itr;
					const auto& qname = qcur.name;
					const auto& tags = qcur.value;
					bool err = false;
					assert(tags.IsArray());
					std::vector<std::string> tagstr;
					for (auto itr2 = tags.Begin(); itr2 != tags.End(); ++itr2) 
					{
						tagstr.push_back(itr2->GetString());
					}
					if(!err)
						mQueries.insert( std::pair<std::string, cQueryBase>( qname.GetString(),cTagQuery(tagstr)));
				}
		}
		else
			ECS.mLog->Wrn("cSystemMgr::from_json: \"Queries\" not found");
	}
}