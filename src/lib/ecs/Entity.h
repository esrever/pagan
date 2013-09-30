#pragma once

#include <bitset>
#include <boost/uuid/uuid.hpp>        
#include <boost/uuid/uuid_generators.hpp>

#include "Event.h"
#include "ecs_config.h"

namespace pgn 
{
	class cEntityMgr;

    //! 
    class cEntity
    {
	private:
		friend class cEntityMgr;
		//sfriend class sptr_type;
		cEntity();

	private:
		boost::uuids::uuid mId;
    };

	DECL_PTRTYPE(cEntity)

	DECL_EVENT(EntityCreated, cEntityPtr);
	DECL_EVENT(DestroyEntity, cEntityPtr);
	DECL_EVENT(ComponentMaskModified, cEntityPtr);

	typedef std::bitset<MAX_COMPONENTS> component_mask_type;
}

bool std::less<pgn::cEntityPtr>::operator ()(const pgn::cEntityPtr &lhs ,const pgn::cEntityPtr &rhs) const
{
	return lhs.lock() < rhs.lock(); 
}