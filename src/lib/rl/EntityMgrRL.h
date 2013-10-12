#pragma once

#include <ecs/EntityMgr.h>

namespace pgn
{
	class cEntityMgrRL : public pgn::cEntityMgr 
	{
	public:
		virtual const std::string ReceiverName() const {return "EntityMgrRL";}
		virtual void RegisterComponentTypes();
	};
}