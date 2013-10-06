#pragma once

#include <set>
#include <core/util/json_conversions.h>

namespace pgn
{
	class cComponentQuery;
	class cSystemBase;

	class cSystemMgr
	{
		public:
			void AddSystem(const cSystemBase& zSystem, int zPriority);
			void RemoveSystem(const cSystemBase& zSystem);

			//! Json
			void from_json(const rapidjson::Value& zRoot);
			
		private:
			//! Component queries: maps tags to entities that have prespecified components
			std::set<cComponentQuery> mComponentQueries;
	};
}