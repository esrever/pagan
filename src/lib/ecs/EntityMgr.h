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
#include "EntityData.h"
#include "EntityGlobals.h"
#include "Archetype.h"
#include "Exemplar.h"

namespace pgn
{
	class cEntityMgr
	{
		public:
			typedef std::function< std::shared_ptr<cComponentBase>() > component_creator_fun;
		public:
			cEntityMgr();
			virtual const std::string ReceiverName() const {return "EntityMgr";}
			virtual ~cEntityMgr(){}

			//! Entity create/destroy
			cEntityWithData Create();
			void Destroy(cEntityWithData zEntity);

			//! Instantiations
			cEntityWithData InstantiateEntity(const std::string& zArchName,const std::string& zExemplarName);
			cExemplar ArchetypeToExemplar(const std::string& zArchName);
			cEntityWithData InstantiateExemplar(const std::string& zExemplarName);
			cEntityWithData CloneEntity(cEntityWithData ed);

			//! Component add/remove
			void AddComponentPtr(cEntityWithData zEntity, cComponentBaseSptr zComponent);
			void RemoveComponentPtr(cEntityWithData zEntity, cComponentBaseWptr zComponent);
			template<class T>
			void AddComponent(cEntityWithData zEntity, const T& zComponent);
			template<class T>
			void RemoveComponent(cEntityWithData zEntity, const T& zComponent);

			//! Entity marking/unmarking functions
			void Tag(cEntityWithData zEntity, const std::string& zTag);
			void Untag(cEntityWithData zEntity, const std::string& zTag);
			void Untag(const std::string& zTag);
			void Untag(cEntityWithData zEntity);
			void TaggedEntities(std::vector<cEntityWithData>& e, const std::string s);
			cEntityWithData TaggedEntity(const std::string s);

			//! Receiving functions
			void OnEntityCreated(cEntityWithData e);
			void OnEntityDestroy(cEntityWithData e);

			//! Accessors
			const cEntityGlobals& Globals() const { return mGlobals; }
			const cEntityData& GetEntityData(const cEntity& zEntity) const;
			const std::map<cEntity, cEntityData>& GetEntityData() const { return mEntityData; }
			std::map<cEntity, cEntityData>& GetEntityData() { return mEntityData; }
			bool IsValid(cEntityWithData ed) const { return ed != mEntityData.end(); }

			//! Json
			bool from_json(const rapidjson::Value& zRoot);
			void ImportArchetypes(const rapidjson::Document * zDoc);
			void ImportInstances(const rapidjson::Document * zDoc, bool zIsExemplar);

			//! Component type related
			virtual void RegisterComponentTypes(){}
			template<class T> void AddComponentType();
			size_t GetComponentTypeIndex( const std::string& zName) const;
			const std::vector< std::type_index>& GetComponentTypeIndexAll( ) const {return mComponentTypeIds;}
			std::shared_ptr<cComponentBase> CreateComponent(const std::string& zName) const;
			std::shared_ptr<cComponentBase> CreateComponent(size_t zIdx) const;

			template<class Writer>
			void Serialize(Writer& writer) const;
	private:
		template<class T>
		friend void to_json(const T& zObj, JsonWriter& zRoot);

		private:
			cEventHandler<evt::cEntityCreated> mOnEntityCreated;
			cEventHandler<evt::cEntityDestroy> mOnEntityDestroy;
		
		private:	
			unsigned short AddComponentType( const std::type_index& zTi);

		protected:
			//! Archetypes
			std::map<std::string, cArchetype> mArchetypes;

			//! Exemplars
			std::map<std::string, cExemplar> mExemplars;

			//! entities and components
			std::map<cEntity, cEntityData> mEntityData;

		protected:
			//! component-related
			std::vector< std::type_index> mComponentTypeIds;
			std::map< std::string, size_t > mComponentTypeNamesToIds;
			std::vector<component_creator_fun> mComponentCreators;

		private:
			cEntityGlobals mGlobals;
			//! entity id generator
			cIdGen<cEntity> mEntityIdGen;
	};

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::AddComponent(cEntityWithData zEntity, const T& zComponent)
	{
		AddComponentPtr(zEntity, std::make_shared<T>(zComponent));
	}

	//------------------------------------------------------------------------
	template<class T>
	void cEntityMgr::RemoveComponent(cEntityWithData zEntity, const T& zComponent)
	{
		RemoveComponentPtr(zEntity, std::make_shared<T>(zComponent));
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

	//------------------------------------------------------------------------
	template<>
	void to_json<cEntityMgr>(const cEntityMgr& zObj, JsonWriter& zRoot);
	
}