#pragma once

#include <map>
#include <memory>
#include <Delegate.h>
#include <core/util/json_conversions.h>

namespace pgn
{
	class cComponentQuery;
	class cSystemBase;
	typedef Gallant::Delegate0< std::shared_ptr<cSystemBase> > system_creator_fun;

	class cSystemMgr
	{
		public:
			typedef std::map<std::string, cComponentQuery> QueryMap;
			typedef std::multimap<size_t, std::shared_ptr<cSystemBase> > SystemMap;
		public:
			virtual const std::string ReceiverName() const {return "SystemMgr";}
			virtual ~cSystemMgr(){}
			void AddSystem(std::shared_ptr<cSystemBase> zSystem, int zPriority);
			void RemoveSystem(std::shared_ptr<cSystemBase> zSystem);

			const QueryMap& GetQueries() const {return mComponentQueries;}

			//! Json
			void from_json(const rapidjson::Value& zRoot);

			std::shared_ptr<cSystemBase> Create(const std::string& zName) const;
			virtual void RegisterSystemTypes(){}
			template<class T>
			void AddSystemType();
			
			
		protected:
			//! Component queries: maps tags to entities that have prespecified components
			QueryMap	mComponentQueries;
			SystemMap	mSystems;

			//! All system types
			std::map< std::string, system_creator_fun> mSystemCreators;
	};

	//---------------------------------------------------------------
	template<class T>
	void cSystemMgr::AddSystemType()
	{
		const std::type_info& ti = typeid(T);
		system_creator_fun func = &cSystemBase::Create<T>;
		std::vector<std::string> res;
		pystring::split(ti.name(),res," ");
		mSystemCreators[res[1]] = func;
	}
}