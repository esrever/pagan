#pragma once

#include <functional>
#include <map>
#include <memory>
#include <core/util/json_conversions.h>

#include "QueryExpression.h"

namespace pgn
{
	class cSystemBase;
	class cEventHandlerQueryBase;
	typedef std::function< std::shared_ptr<cSystemBase> (void) > system_creator_fun;

	class cSystemMgr
	{
		public:
			typedef std::map<std::string, cQueryExpressionSptr> QueryMap;
			typedef std::multimap<size_t, std::shared_ptr<cSystemBase> > SystemMap;
		public:
			virtual ~cSystemMgr(){}

			//! System add/remove
			void AddSystem(std::shared_ptr<cSystemBase> zSystem, int zPriority);
			void RemoveSystem(std::shared_ptr<cSystemBase> zSystem);

			//! Queries
			const QueryMap& GetQueries() const {return mQueries;}
			void AddQuery(const std::string& zName, const cQueryExpressionSptr& zPtr);
			cQueryExpressionSptr GetQuery(const std::string& zName);


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

			virtual void RegisterAllEvents(){}
			
		private:
			template<class T>
			friend void to_json(const T& zObj, JsonWriter& zRoot);
			
		protected:
			QueryMap	mQueries;
			SystemMap	mSystems;

			//! Event handlers
			std::vector<std::shared_ptr<cEventHandlerQueryBase>> mEventHandlerQueries;

			//! All system types
			std::map< std::string, system_creator_fun> mSystemCreators;
			std::map<std::string, std::shared_ptr<cEventHandlerQueryBase>> mAllEventHandlerQueries;
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

	//---------------------------------------------------------------
	template<>
	void to_json<cSystemMgr>(const cSystemMgr& zMgr, JsonWriter& writer);
}