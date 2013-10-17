#pragma once

#include <ecs/SystemMgr.h>
#include <rl/events/events.h>

namespace pgn
{
	class cSystemMgrRL : public cSystemMgr,
						 public cEventReceiver<cExitApplicationEventData>
	{
	public:
		virtual void RegisterSystemTypes();
		virtual const std::string ReceiverName() const {return "SystemMgrRL";}
		virtual bool UpdateFrame();

		void Receive(const cExitApplicationEventData&);
	
	private:
		bool mLoop;

	};
}