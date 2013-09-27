#pragma once

#include <map>
#include <string>
#include <set>

#include "Entity.h"

namespace pgn
{
	class cComponentBase;

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


			// Component-related functions
			template<class T>
			void AddComponent(cEntityPtr zEntity, const T& zComponent = T());
			void AddComponent(cEntityPtr zEntity, const cComponentBase& zComponent);
			void AddComponent(cEntityPtr zEntity, int zBit);
			
			void RemoveComponent(cEntityPtr zEntity, const cComponentBase& zComponent);
			void RemoveComponent(cEntityPtr zEntity, int zBit);


			//! Entity marking/unmarking functions
			void Tag(cEntityPtr zEntity, const std::string& zTag);
			void Untag(cEntityPtr zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(cEntityPtr zEntity);

			//! Receiving functions
			void Receive( const cEntityCreatedEventData& zData);
			void Receive( const cDestroyEntityEventData& zData);
			void Receive( const cComponentMaskModifiedEventData& zData);

		private:
			//! All entities
			std::set<cEntitySptr> mEntities;
			//! tags to entities
			std::map<std::string, std::set<cEntityPtr>> mTaggedEntities;
			//! entities to component masks
			std::map<cEntityPtr, std::bitset<MAX_COMPONENTS>> mComponentMasks;
	};

	
}