#pragma once

namespace pgn
{
	class cECS
	{
		public:

		private:
			// sub-managers
			cEntityMgr mEntityMgr;
			cSystemMgr mSystemMgr;
			
			std::map<string, cComponentQuery> mComponentQueries;
	}
}