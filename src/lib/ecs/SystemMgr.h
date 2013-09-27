#pragma once

namespace pgn
{
	class cComponentQuery;

	class cSystemMgr
	{
		public:
			void AddSystem(const cSystemBase& zSystem, int zPriority);
			void RemoveSystem(const cSystemBase& zSystem);
			
		private:
			std::map<std::string, cComponentQuery> mComponentQueries;
	};
}