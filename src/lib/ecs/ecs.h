#pragma once

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <string>
#include <typeinfo>
#include <typeindex>

#include <core/util/singleton.h>
#include <core/util/idgen.h>

#include "ecs-config.h"
#include "component.h"
#include "EntityData.h"

#define DECL_MAP_MEMBER_R(A,B, N) \
	public: const std::map< A , B > & N () const { return m##N ; }\
	private: std::map< A , B > m##N ; 
#define DECL_MEMBER_R(T, N) \
	public: const T & N () const { return m##N ; }\
	private: T m##N ; 

namespace pgn
{
	typedef std::function< std::shared_ptr<cComponentBase>() > ComponentCreatorFunction;

	class cECS
	{
		DECL_MAP_MEMBER_R(cEntity, cEntityData, EntitiesToData);
		DECL_MAP_MEMBER_R(std::string, cEntityData, Archetypes);
		DECL_MAP_MEMBER_R(std::string, std::set<cEntity>, TagsToEntities);
		DECL_MAP_MEMBER_R(std::string, cQueryFunc, Queries);
		DECL_MAP_MEMBER_R(std::string, size_t, ComponentTypeNamesToIds);
		DECL_MEMBER_R(std::vector<ComponentCreatorFunction>, ComponentCreators);

		SUPPORT_DERIVED(cECS);

	public:
		typedef std::map<cEntity, cEntityData>::const_iterator  cEntityWithDataConst;
		typedef std::map<cEntity, cEntityData>::iterator        cEntityWithData;

	public:
		cEntityWithData Create(const cEntityData& = cEntityData());
		void Tag(const std::string&, cEntityWithData);
		void Untag(const std::string&, cEntityWithData);

		cEntityWithData InstantiateArchetype(const cEntityData& arch);

	protected:
		template<class T>
		void RegisterComponent();

	private:
		unsigned short AddComponentType(const std::type_index& zTi);
		void ParseEntities(const node_type& reader);

	private:
		cIdGen<cEntity>						  mEntityIdGen;
		std::vector< std::type_index>		  mComponentTypeIds;
	};

	static inline cECS& ECS() { return pgn::cSingleton<pgn::cECS>::Instance(); }

	DECL_SERIALIZE_INTERFACE(cECS);


	//------------------------------------------------------------------------
	template<class T>
	void cECS::RegisterComponent()
	{
		// set index and store creator function
		const auto& ti = typeid(T);
		cComponent<typename T>::msTypeIndex = AddComponentType(ti);
		mComponentCreators[cComponent<typename T>::msTypeIndex] = &cComponent<typename T>::Create;
	}
}

#undef DECL_MAP_MEMBER_R