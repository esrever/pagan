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

/*
	Init() function
		initializes all components
		Loads json file for queries, systems, etc
	Component data types
		declare a static var in cpp file to populate ECS (at least for now)
		type idx from name 
	Pointers and structs in containers issues:
		tags & componentquery currently duplicate entities
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
			virtual const std::string ReceiverName() const {return "EntityMgr";}
			virtual ~cEntityMgr(){}
			// Entity creation functions
			//! From an existing entity
			cEntitySptr Create();
			void Destroy(cEntityWptr zEntity);

			// Component-related functions - if an entity has no components, it gets destroyed
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

			//! Do nothing
			void Receive( const cEntityCreatedEventData& zData);
			//! Untag
			void Receive( const cDestroyEntityEventData& zData);

			//! Accessors
			const cEntityComponents& GetComponents(const cEntity& zEntity) const;
			const std::map<cEntity, cEntityComponents>& GetComponents() const;

			//! Json
			void from_json(const rapidjson::Value& zRoot){}

			//! Component type
			template<class T>
			void AddComponentType();
			size_t GetComponentTypeIndex( const std::string& zName) const;
		
		private:	
			unsigned short AddComponentType( const std::type_index& zTi);

		protected:
			//! tags to entities
			std::map<std::string, std::set<cEntity>> mTaggedEntities;
			//! entities and components
			std::map<cEntity, cEntityComponents> mEntityComponents;

		protected:
			//! All component types
			std::vector< std::type_index> mComponentTypeIds;
			std::map< std::string, size_t > mComponentTypeNamesToIds;
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

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::AddComponentType()
	{
		cComponent<T>::msTypeIndex = AddComponentType(typeid(T));
	}
}