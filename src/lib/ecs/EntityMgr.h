#pragma once

#include <bitset>
#include <map>
#include <string>
#include <set>

#include "Entity.h"

namespace pgn
{
	typedef std::bitset<MAX_COMPONENTS> component_mask_type;

	DECL_EVENT(EntityCreated, cEntity);
	DECL_EVENT(DestroyEntity, cEntity);
	DECL_EVENT(ComponentMaskModified, cEntity);

	class cEntityMgr : public cEventReceiver<cEntityCreatedEventData>,
					   public cEventReceiver<cDestroyEntityEventData>,
					   public cEventReceiver<cComponentMaskModifiedEventData>
	{
		public:
			// Entity creation functions

			//! From an existing entity
			cEntity Create( const cEntity& zEntity = cEntity());
			cEntity Create( const component_mask_type& zMask, 
							const cEntity& zEntity = cEntity()); // augment
			//cEntity Create( conf = 0, e = null);  // augment
			//cEntity Create( bin = 0, e = null);   // augment

			void Destroy(const cEntity& zEntity);


			// Entity marking/unmarking functions
			void Tag(const cEntity& zEntity, const std::string& zTag);
			void Untag(const cEntity& zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(const cEntity& zEntity);

			// Receiving functions
			void Receive( const cEntityCreatedEventData& zData);
			void Receive( const cDestroyEntityEventData& zData);
			void Receive( const cComponentMaskModifiedEventData& zData);

		private:
			std::map<std::string, std::set<cEntity>> mTaggedEntities;
			std::map<cEntity, std::bitset<MAX_COMPONENTS>> mComponentMasks;
	};

	
}