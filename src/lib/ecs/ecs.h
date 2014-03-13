#pragma once

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <string>
#include <typeinfo>
#include <typeindex>

#include <core/app/sdlapp.h>
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
	public:
		typedef std::map<cEntity, cEntityData>::const_iterator  cEntityWithDataConst;
		typedef std::map<cEntity, cEntityData>::iterator        cEntityWithData;
		typedef std::map<std::string, cEntityData>::const_iterator        cArchetypeWithDataConst;
		typedef std::map<std::string, cEntityData>::iterator        cArchetypeWithData;

	public:
		DECL_MAP_MEMBER_R(cEntity, cEntityData, EntitiesToData);
		DECL_MAP_MEMBER_R(std::string, cEntityData, Archetypes);
		DECL_MAP_MEMBER_R(std::string, std::set<cEntityWithData>, TagsToEntities);
		DECL_MAP_MEMBER_R(std::string, cQueryFunc, Queries);
		DECL_MAP_MEMBER_R(std::string, size_t, ComponentTypeNamesToIds);
		DECL_MEMBER_R(std::vector<ComponentCreatorFunction>, ComponentCreators);
		DECL_MEMBER_R(std::vector<std::string>, ComponentTypeNames);

		SUPPORT_DERIVED(cECS);

	public:
		cEntityWithData Create(const cEntityData& = cEntityData());
		void Tag(const std::string&, cEntityWithData);
		void Untag(const std::string&, cEntityWithData);

		cEntityWithData InstantiateArchetype(const cEntityData& arch);

		template<class T>
		void RegisterComponent();

	private:
		unsigned short AddComponentType(const std::type_index& zTi);
		void ParseEntities(const node_type& reader);

	private:
		cIdGen<cEntity>						  mEntityIdGen;
		std::vector< std::type_index>		  mComponentTypeIds;
	};

	cECS& ECS();

	DECL_SERIALIZE_INTERFACE(cECS);


	//------------------------------------------------------------------------
	template<class T>
	void cECS::RegisterComponent()
	{
		// set index and store creator function
		const auto& ti = typeid(T);
		auto idx = AddComponentType(ti);
		cComponent<typename T>::msTypeIndex = idx;
		mComponentCreators[cComponent<typename T>::msTypeIndex] = &cComponent<typename T>::Create;
	}
}

namespace std
{
	inline bool operator < (const pgn::cECS::cEntityWithData& lhs, const pgn::cECS::cEntityWithData& rhs)
	{
		return &lhs < &rhs;
	}
}

#undef DECL_MAP_MEMBER_R
#undef DECL_MEMBER_R