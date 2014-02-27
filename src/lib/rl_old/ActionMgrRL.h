#pragma once

#include <ecs/ActionMgr.h>

namespace pgn
{
	class cActionMgrRL : public cActionMgr
	{
	public:
		virtual void RegisterActionTypes();
		virtual const std::string ReceiverName() const {return "ActionMgrRL";}
	
	private:

	};
}