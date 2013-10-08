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
			virtual const std::string ReceiverName() const {return "SystemMgr";}
			virtual ~cSystemMgr(){}
			void AddSystem(const cSystemBase& zSystem, int zPriority);
			void RemoveSystem(const cSystemBase& zSystem);

			//! Json
			void from_json(const rapidjson::Value& zRoot);

			virtual void RegisterSystemTypes(){}
			template<class T>
			void AddSystemType();
			
		protected:
			//! Component queries: maps tags to entities that have prespecified components
			std::map<std::string, cComponentQuery> mComponentQueries;
			std::map<size_t, std::shared_ptr<cSystemBase> > mSystems;

			//! All system types
			std::map< std::string, system_creator_fun> mSystemCreators;
	};

	//---------------------------------------------------------------
	template<class T>
	void cSystemMgr::AddSystemType()
	{
		const std::type_info& ti = typeid(T);
		system_creator_fun func = &cSystemBase::Create<T>;
		mSystemCreators[ti.name()] = func;
	}
}