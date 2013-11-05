#pragma once

#include <ecs/SystemMgr.h>
#include <rl/events/events.h>

namespace pgn
{
	class cSystemMgrRL : public cSystemMgr
	{
	public:
		cSystemMgrRL();
		virtual void RegisterSystemTypes();
		virtual bool UpdateFrame();

		void OnExitApplication();
	private:
		cEventHandler<cExitApplicationEvent> mOnExitApplication;
	private:
		bool mLoop;

	};
}