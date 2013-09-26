#pragma once

#include <bitset>
#include <map>
#include <string>
#include <set>

#include "Entity.h"

namespace pgn
{
	typedef std::bitset<MAX_COMPONENTS> component_mask_type;

	DECL_EVENT(EntityCreated, cEntityPtr);
	DECL_EVENT(DestroyEntity, cEntityPtr);
	DECL_EVENT(ComponentMaskModified, cEntityPtr);

	class cEntityMgr : public cEventReceiver<cEntityCreatedEventData>,
					   public cEventReceiver<cDestroyEntityEventData>,
					   public cEventReceiver<cComponentMaskModifiedEventData>
	{
		public:
			// Entity creation functions

			//! From an existing entity
			cEntityPtr Create( cEntityPtr = cEntity::wptr_type() );
			cEntityPtr Create( const component_mask_type& zMask, 
							cEntityPtr = cEntity::wptr_type());
			//cEntity Create( conf = 0, e = null);  // augment
			//cEntity Create( bin = 0, e = null);   // augment

			void Destroy(cEntityPtr zEntity);


			// Entity marking/unmarking functions
			void Tag(cEntityPtr zEntity, const std::string& zTag);
			void Untag(cEntityPtr zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(cEntityPtr zEntity);

			// Receiving functions
			void Receive( const cEntityCreatedEventData& zData);
			void Receive( const cDestroyEntityEventData& zData);
			void Receive( const cComponentMaskModifiedEventData& zData);

		private:
			std::set<cEntitySptr> mEntities;
			std::map<std::string, std::set<cEntityPtr>> mTaggedEntities;
			std::map<cEntityPtr, std::bitset<MAX_COMPONENTS>> mComponentMasks;
	};

	
}