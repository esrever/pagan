#pragma once

#include <ecs/SystemMgr.h>

namespace pgn
{
	class cSystemMgrRL : public cSystemMgr
	{
	public:
		virtual void RegisterSystemTypes();
		virtual const std::string ReceiverName() const {return "SystemMgrRL";}

	};
}