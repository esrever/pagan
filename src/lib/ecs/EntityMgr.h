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
#include <core/util/json_conversions.h>

#include "ecs_config.h"
#include "Entity.h"
#include "Event.h"
#include "Component.h"
#include "EntityComponents.h"
#include "Archetype.h"

/*
TODO:
	Would I have multiple components of the same type per entity?
		No. Items are stored as "ItemList"
	Components can be referenced by other components
		Sword component by inventory and equipment components
*/

namespace pgn
{
	class cEntityMgr
	{
		public:
			typedef std::map<std::string, std::set<cEntity>> tagged_entity_map;
			typedef std::function< std::shared_ptr<cComponentBase>() > component_creator_fun;
		public:
			cEntityMgr();
			virtual const std::string ReceiverName() const {return "EntityMgr";}
			virtual ~cEntityMgr(){}

			//! Entity create/destroy
			cEntity Create();
			void Destroy(cEntity zEntity);
			cEntity InstantiateEntity(const std::string& zArchName,const std::string& zExemplarName);
			cEntity InstantiateArchetype(const std::string& zArchName);
			cEntity InstantiateExemplar(const std::string& zExemplarName);

			//! Component add/remove
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

			//! Receiving functions
			void OnEntityCreated(cEntity e);
			void OnEntityDestroy(cEntity e);

			//! Accessors
			const cEntityComponents& GetComponents(const cEntity& zEntity) const;
			const std::map<cEntity, cEntityComponents>& GetComponents() const;
			const tagged_entity_map& TaggedEntities() const {return mTaggedEntities;}

			//! Json
			bool from_json(const rapidjson::Value& zRoot);
			void ImportArchetypes(const rapidjson::Document * zDoc);
			void ImportInstances(const rapidjson::Document * zDoc, bool isExemplar);

			//! Component type related
			virtual void RegisterComponentTypes(){}
			template<class T> void AddComponentType();
			size_t GetComponentTypeIndex( const std::string& zName) const;
			const std::vector< std::type_index>& GetComponentTypeIndexAll( ) const {return mComponentTypeIds;}
			std::shared_ptr<cComponentBase> CreateComponent(const std::string& zName) const;
			std::shared_ptr<cComponentBase> CreateComponent(size_t zIdx) const;

		private:
			cEventHandler<cEntityCreatedEvent> mOnEntityCreated;
			cEventHandler<cEntityDestroyEvent> mOnEntityDestroy;
		
		private:	
			unsigned short AddComponentType( const std::type_index& zTi);

		protected:
			//! Archetypes
			std::map<std::string, cArchetype> mArchetypes;

			//! Exemplars
			std::map<std::string, cEntity> mExemplars;

			//! tags to entities
			std::map<std::string, std::set<cEntity>> mTaggedEntities;
			//! entities and components
			std::map<cEntity, cEntityComponents> mEntityComponents;

		protected:
			//! component-related
			std::vector< std::type_index> mComponentTypeIds;
			std::map< std::string, size_t > mComponentTypeNamesToIds;
			std::vector<component_creator_fun> mComponentCreators;

		private:
			//! entity id generator
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
		const auto& ti = typeid(T);
		cComponent<typename T>::msTypeIndex = AddComponentType(ti);
		component_creator_fun func = &cComponent<typename T>::Create;
		mComponentCreators[cComponent<typename T>::msTypeIndex] = func;
	}
}