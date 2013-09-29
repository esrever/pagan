#pragma once

#include <map>
#include <memory>
#include <string>
#include <set>
#include <unordered_set>
#include <typeinfo>
#include <typeindex>
#include <vector>

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


			// TODO: add ctros from templated component and bitid
			// Component-related functions
			void AddComponent(cEntityPtr zEntity, std::shared_ptr<cComponentBase> zComponent); 
			void RemoveComponent(cEntityPtr zEntity, std::weak_ptr<cComponentBase> zComponent); 

			//! Entity marking/unmarking functions
			void Tag(cEntityPtr zEntity, const std::string& zTag);
			void Untag(cEntityPtr zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(cEntityPtr zEntity);

			//! Receiving functions
			void Receive( const cEntityCreatedEventData& zData);
			void Receive( const cDestroyEntityEventData& zData);
			void Receive( const cComponentMaskModifiedEventData& zData);

			unsigned short AddComponentType( const std::type_index& zTi);

		private:
			//! All entities
			std::set<cEntitySptr> mEntities;
			//! tags to entities
			std::map<std::string, std::set<cEntityPtr>> mTaggedEntities;
			//! entities to component masks
			std::map<cEntityPtr, std::bitset<MAX_COMPONENTS>> mComponentMasks;
			//! All component types
			std::vector< std::type_index> mComponentTypeIds;
			//! All components

			typedef std::shared_ptr<cComponentBase> ComponentSptr;
			typedef std::unordered_set<ComponentSptr> ComponentSet;
			std::map<cEntitySptr, ComponentSet> mComponents;
	};
}