#pragma once

#include <functional>
#include <map>
#include <memory>
#include <core/util/json_conversions.h>

#include "QueryExpression.h"

namespace pgn
{
	class cSystemBase;
	typedef std::function< std::shared_ptr<cSystemBase> (void) > system_creator_fun;

	class cSystemMgr
	{
		public:
			typedef std::map<std::string, cQueryExpression> QueryMap;
			typedef std::multimap<size_t, std::shared_ptr<cSystemBase> > SystemMap;
		public:
			virtual const std::string ReceiverName() const {return "SystemMgr";}
			virtual ~cSystemMgr(){}

			//! System add/remove
			void AddSystem(std::shared_ptr<cSystemBase> zSystem, int zPriority);
			void RemoveSystem(std::shared_ptr<cSystemBase> zSystem);

			//! Queries
			const QueryMap& GetQueries() const {return mQueries;}
			const std::string& GetQueryName(const cQueryExpression& zQuery) const;

			//! the main frame updater
			virtual bool UpdateFrame(){return true;}

			//! Json-related 
			bool from_json(const rapidjson::Value& zRoot);
			void ImportSystems(const rapidjson::Document * zDoc);
			void ImportQueries(const rapidjson::Document * zDoc);

			//! pointer generator
			std::shared_ptr<cSystemBase> Create(const std::string& zName) const;

			//! System type related
			virtual void RegisterSystemTypes(){}
			template<class T>
			void AddSystemType();
			
			
		protected:
			QueryMap	mQueries;
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