#pragma once

namespace pgn
{
	class cSystemMgr
	{
		public:
			void AddSystem(const cSystemBase);
			void RemoveSystem(const cSystemBase);
	};
}