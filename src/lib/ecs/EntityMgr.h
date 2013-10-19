#pragma once

#include <map>
#include <memory>
#include <string>
#include <set>
#include <unordered_set>
#include <typeinfo>
#include <typeindex>
#include <vector>

#include <core/util/idgen.h>

#include "ecs_config.h"
#include "Entity.h"
#include "Event.h"
#include "Component.h"
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
			typedef std::map<std::string, std::set<cEntity>> tagged_entity_map;
		public:
			virtual void RegisterComponentTypes(){}
			virtual const std::string ReceiverName() const {return "EntityMgr";}
			virtual ~cEntityMgr(){}
			// Entity creation functions
			//! From an existing entity
			cEntity Create();
			void Destroy(cEntity zEntity);

			// Component-related functions - if an entity has no components, it gets destroyed
			void AddComponentPtr(cEntity zEntity, cComponentBaseSptr zComponent); 
			void RemoveComponentPtr(cEntity zEntity, cComponentBaseWptr zComponent); 
			template<class T>
			void AddComponent(cEntity zEntity, const T& zComponent); 
			template<class T>
			void RemoveComponent(cEntity zEntity, const T& zComponent); 

			//! Entity marking/unmarking functions
			void Tag(cEntity zEntity, const std::string& zTag);
			void Untag(cEntity zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(cEntity zEntity);

			const tagged_entity_map& TaggedEntities() const {return mTaggedEntities;}

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
			const std::vector< std::type_index>& GetComponentTypeIndexAll( ) const {return mComponentTypeIds;}
		
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

		private:
			cIdGen<cEntity> mEntityIdGen;
	};

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::AddComponent(cEntity zEntity, const T& zComponent)
	{
		AddComponentPtr(zEntity, std::make_shared<T>(zComponent) );
	}

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::RemoveComponent(cEntity zEntity, const T& zComponent)
	{
		RemoveComponentPtr(zEntity, std::make_shared<T>(zComponent) );
	}

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::AddComponentType()
	{
		cComponent<typename T>::msTypeIndex = AddComponentType(typeid(T));
	}
}