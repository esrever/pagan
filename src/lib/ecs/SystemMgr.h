#pragma once

#include <map>
#include <core/util/json_conversions.h>

namespace pgn
{
	class cComponentQuery;
	class cSystemBase;

	class cSystemMgr
	{
		public:
			virtual ~cSystemMgr(){}
			void AddSystem(const cSystemBase& zSystem, int zPriority);
			void RemoveSystem(const cSystemBase& zSystem);
			virtual std::shared_ptr<cSystemBase> Create(const std::string& zName,
												const std::string& zType,
												const std::string& zDesc,
												const std::vector<std::string>& zQueriesUsed,
												const rapidjson::Value& zInitData);

			//! Json
			void from_json(const rapidjson::Value& zRoot);
			
		protected:
			//! Component queries: maps tags to entities that have prespecified components
			std::map<std::string, cComponentQuery> mComponentQueries;
			std::map<size_t, std::shared_ptr<cSystemBase> > mSystems;
	};
}