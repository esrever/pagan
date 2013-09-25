#pragma once

#include <bitset>

#include "Event.h"
#include "ecs_config.h"

namespace pgn 
{
	class cEntityMgr;

    //! 
    class cEntity
    {
		friend class cEntityMgr;
		public:
			typedef std::bitset<MAX_COMPONENTS> component_mask_type;

		public:
			cEntity():mId(0){}

			// TODO: correct comparison
			bool operator < (const cEntity& e) const { return mId < e.mId; }
		
		//! Members
		private:	
			//! the basic id 
			unsigned long mId;
			//! the associated components
			component_mask_type mComponentMask;
		
		//! Static members
		private:
			//! The basic id generator, resets every app run	
			static unsigned long msIdCounter;
    };

	DECL_EVENT(EntityCreated, cEntity);
	DECL_EVENT(DestroyEntity, cEntity);

	typedef std::pair<cEntity, unsigned> component_of_entity;
	//DECL_EVENT(ComponentAdded, component_of_entity);
	//DECL_EVENT(ComponentRemoved, component_of_entity);
}