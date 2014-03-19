#pragma once

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <string>
#include <typeinfo>
#include <typeindex>

#include <pystring.h>

#include <core/app/sdlapp.h>
#include <core/util/singleton.h>
#include <core/util/idgen.h>

#include "ecs-config.h"
#include "component.h"
#include "EntityData.h"
#include "system.h"

#define DECL_MAP_MEMBER_R(A,B, N) \
	public: const std::map< A , B > & N () const { return m##N ; }\
	public: std::map< A , B >::iterator N (const A & v) { return m##N##.find(v); }\
	private: std::map< A , B > m##N ; 
#define DECL_MEMBER_R(T, N) \
	public: const T & N () const { return m##N ; }\
	private: T m##N ; 

namespace pgn
{
	namespace ecs
	{
		typedef std::function< std::shared_ptr<cComponentBase>() >	ComponentCreatorFunction;
		typedef std::map<cEntity, cEntityData>::const_iterator		cEntityWithDataConst;
		typedef std::map<cEntity, cEntityData>::iterator			cEntityWithData;
		typedef std::function<void(void)>							cActionFunc;
		typedef std::map<int, cActionFunc> 							cActionBindings;
		typedef std::map<std::string, cEntityData>::const_iterator  cArchetypeWithDataConst;
		typedef std::map<std::string, cEntityData>::iterator        cArchetypeWithData;

		class cECS
		{

		public:
			DECL_MAP_MEMBER_R(cEntity, cEntityData, EntitiesToData);
			DECL_MAP_MEMBER_R(std::string, cEntityData, Archetypes);
			DECL_MAP_MEMBER_R(std::string, std::set<cEntityWithData>, TagsToEntities);
			DECL_MAP_MEMBER_R(std::string, cEntityWithData, TagusToEntities);
			DECL_MAP_MEMBER_R(std::string, cQueryFunc, Queries);
			DECL_MAP_MEMBER_R(std::string, size_t, ComponentTypeNamesToIds);
			DECL_MEMBER_R(std::vector<ComponentCreatorFunction>, ComponentCreators);
			DECL_MEMBER_R(std::vector<std::string>, ComponentTypeNames);
			DECL_MAP_MEMBER_R(std::string, cActionFunc, ActionFuncs);

			SUPPORT_DERIVED(cECS);

		public:
			cEntityWithData Create(const cEntityData& = cEntityData());
			void Destroy(cEntityWithData);
			void Tag(const std::string&, cEntityWithData);
			void Untag(const std::string&, cEntityWithData);
			void Tagu(const std::string&, cEntityWithData);
			void Untagu(const std::string&, cEntityWithData);

			cEntityWithData InstantiateArchetype(const cEntityData& arch);

			template<class T>
			void RegisterComponent();

			//! register an action
			void RegisterAction(const std::string& s, cActionFunc func);

			//! gets a typed system - creates if none exists.
			template<class T>
			ecs::sys::cBase_sptr System();

			void ParseEntities(const node_type& reader);

		private:
			unsigned short AddComponentType(const std::type_index& zTi);
			ecs::sys::cBase_sptr& SystemBase(const std::string& s) { return mSystems[s]; }

		private:
			std::map<std::string, ecs::sys::cBase_sptr>	mSystems;

			cIdGen<cEntity>						  mEntityIdGen;
			std::vector< std::type_index>		  mComponentTypeIds;
		};

		cECS& ECS();

		//------------------------------------------------------------------------
		template<class T>
		sys::cBase_sptr cECS::System()
		{
			auto name = typeid(T).name();
			// create a nice name & add it, omit the struct from "struct blah"
			std::vector<std::string> result;
			pystring::rsplit(name, result, "::c", 1);
			auto s = pystring::strip(result.back(), ">");

			auto& base = SystemBase(s);
			if (!base)
				base = ecs::sys::cBase_sptr(new T());
			return base;
		}

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

	DECL_SERIALIZE_INTERFACE(ecs::cECS);
	DECL_SERIALIZE_INTERFACE( ecs::cActionBindings );
}

namespace std
{
	inline bool operator < (const pgn::ecs::cEntityWithData& lhs, const pgn::ecs::cEntityWithData& rhs)
	{
		return lhs->first < rhs->first;
	}

	inline bool operator < (const pgn::ecs::cEntityWithDataConst lhs, const pgn::ecs::cEntityWithDataConst rhs)
	{
		return lhs->first < rhs->first;
	}
}

#undef DECL_MAP_MEMBER_R
#undef DECL_MEMBER_R