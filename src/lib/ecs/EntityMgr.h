#pragma once

#include <map>
#include <memory>
#include <string>
#include <set>
#include <unordered_set>
#include <typeinfo>
#include <typeindex>
#include <vector>

#include "ecs_config.h"
#include "Entity.h"
#include "Event.h"
#include "ComponentBase.h"
#include "EntityComponents.h"
#include "ComponentQuery.h"

#include <core/util/Singleton.h>
#define ECS pgn::cSingleton<pgn::cEntityMgr>::Instance()

/*
	Naming:
		blueprints are randomisable, instanced entities
		blueprints have names
		blueprints can be merged/inher
		default blueprint is "default"
		entities from blue prints get a successive number: orc_shaman_32

	How do I do erase() with finding actually the correct data?
	Would I have multiple components of the same type per entity?
		No. Items are stored as "ItemList"
	Components can be referenced by other components
		Sword component by inventory and equipment components
	Use the lock-check of weak pointers to handle markDeleted, etc. cases
	So the componentQueries, instead of lists, they can use other containers of weak ptrs?
		No, as they would still need to do extra processing in the end to weed out null weak_ptrs
*/

namespace pgn
{
	class cEntityMgr : public cEventReceiver<cEntityCreatedEventData>,
					   public cEventReceiver<cDestroyEntityEventData>
	{
		public:
			// Entity creation functions
			//! From an existing entity
			cEntitySptr Create();
			/*
			cEntityPtr Create( const component_mask_type& zMask, 
							cEntityPtr = cEntity::wptr_type());
							*/
			//cEntity Create( conf = 0, e = null);  // augment
			//cEntity Create( bin = 0, e = null);   // augment

			void Destroy(cEntityWptr zEntity);

			// Component-related functions 
			void AddComponentPtr(cEntityWptr zEntity, cComponentBaseSptr zComponent); 
			void RemoveComponentPtr(cEntityWptr zEntity, cComponentBaseWptr zComponent); 
			template<class T>
			void AddComponent(cEntityWptr zEntity, const T& zComponent); 
			template<class T>
			void RemoveComponent(cEntityWptr zEntity, const T& zComponent); 

			//! Entity marking/unmarking functions
			void Tag(cEntityWptr zEntity, const std::string& zTag);
			void Untag(cEntityWptr zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(cEntityWptr zEntity);

			//! Receiving functions
			void Receive( const cEntityCreatedEventData& zData);
			void Receive( const cDestroyEntityEventData& zData);

			unsigned short AddComponentType( const std::type_index& zTi);

		private:
			//! tags to entities
			std::map<std::string, std::set<cEntityWptr>> mTaggedEntities;
			//! entities and components
			std::map<cEntity, cEntityComponents> mEntityComponents;
			//! Component queries: maps tags to entities that have prespecified components
			std::map<std::string, cComponentQuery> mComponentQueries;
			
			//! Counters for blueprints
			std::map<std::string, unsigned> mBlueprintCounters;

		private:
			//! All component types
			std::vector< std::type_index> mComponentTypeIds;
	};

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::AddComponent(cEntityWptr zEntity, const T& zComponent)
	{
		AddComponentPtr(zEntity, std::make_shared<T>(zComponent) );
	}

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::RemoveComponent(cEntityWptr zEntity, const T& zComponent)
	{
		RemoveComponentPtr(zEntity, std::make_shared<T>(zComponent) );
	}
}