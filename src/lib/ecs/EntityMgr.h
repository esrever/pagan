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

/*
	Entity is stored by value
	Components are stored as shared_pointers (cComponentBase)
		add comparison operators: *(sptr.get())
		how do I do erase() with finding actually the correct data?
	Would I have multiple components of the same type per entity?
		No. Items are stored as "ItemList"
	Components can be referenced by other components
		Sword component by inventory and equipment components

	figure out how to give weak_ptrs. or emit shared ptrs directly?
	Use the lock-check of weak pointers to handle markDeleted, etc. cases
	So the componentQueries, instead of lists, they can use other containers of weak ptrs?
		No, as they would still need to do extra processing in the end to weed out null weak_ptrs
*/

namespace pgn
{
	class cComponentBase;
	DECL_PTRTYPE(cComponentBase);

	class cEntityComponents;

	class cEntityMgr : public cEventReceiver<cEntityCreatedEventData>,
					   public cEventReceiver<cDestroyEntityEventData>,
					   public cEventReceiver<cComponentMaskModifiedEventData>
	{
		public:
			// Entity creation functions
			//! From an existing entity
			/*
			cEntityPtr Create( cEntityPtr = cEntity::wptr_type() );
			cEntityPtr Create( const component_mask_type& zMask, 
							cEntityPtr = cEntity::wptr_type());
							*/
			//cEntity Create( conf = 0, e = null);  // augment
			//cEntity Create( bin = 0, e = null);   // augment

			void Destroy(cEntityCref zEntity);


			// TODO: add ctros from templated component and bitid
			// Component-related functions
			void AddComponent(cEntityCref zEntity, cComponentBaseSptr zComponent); 
			void RemoveComponent(cEntityCref zEntity, cComponentBaseWptr zComponent); 

			//! Entity marking/unmarking functions
			void Tag(cEntityCref zEntity, const std::string& zTag);
			void Untag(cEntityCref zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(cEntityCref zEntity);

			//! Receiving functions
			void Receive( const cEntityCreatedEventData& zData);
			void Receive( const cDestroyEntityEventData& zData);
			void Receive( const cComponentMaskModifiedEventData& zData);

			unsigned short AddComponentType( const std::type_index& zTi);

		private:
			//! tags to entities
			std::map<std::string, std::set<cEntityWptr>> mTaggedEntities;
			//! entities and components
			std::map<cEntity, cEntityComponents> mEntityComponents;
			//! All component types
			std::vector< std::type_index> mComponentTypeIds;
	};
}