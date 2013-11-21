#pragma once

#include <ecs/EntityMgr.h>

namespace pgn
{
	struct cGlobalEntities
	{
		cEntity mPlayer;
		cEntity mWorld;
	};

	class cEntityMgrRL : public pgn::cEntityMgr 
	{
	public:
		virtual const std::string ReceiverName() const {return "EntityMgrRL";}
		virtual void RegisterComponentTypes();

		cGlobalEntities mGlobalEntities;
	};
}