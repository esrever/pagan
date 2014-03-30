#pragma once

#include <vector>
#include <map>
#include <unordered_set>
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
	public: std::map< A, B > & N() { return m##N; }\
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

		struct cEntityWithDataHash
		{
			size_t operator()(const pgn::ecs::cEntityWithData& it) const { return it->first; }
		};
		struct cEntityWithDataEq
		{
			bool operator()(const pgn::ecs::cEntityWithData& lhs, const pgn::ecs::cEntityWithData& rhs) const		{	return lhs->first == rhs->first;	}
		};
		typedef std::unordered_set<cEntityWithData, cEntityWithDataHash, cEntityWithDataEq>					cEntityWithDataSet;

		class cECS
		{

		public:
			DECL_MAP_MEMBER_R(cEntity, cEntityData, EntitiesToData);
			DECL_MAP_MEMBER_R(std::string, cEntityData, Archetypes);
			DECL_MAP_MEMBER_R(std::string, cEntityWithDataSet, TagsToEntities);
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

			//! Instantiates an archetype, copying all its data.
			cEntityWithData InstantiateArchetype(const cEntityData& arch);

			//! Instantiates a component, either non-existent or shared. If shared, copies the value.
			template<class T>
			T * InstantiateComponent(cEntityWithData& ed);

			template<class T>
			void RegisterComponent();

			//! register an action
			void RegisterAction(const std::string& s, cActionFunc func);

			//! gets a typed system - creates if none exists.
			template<class T>
			ecs::sys::cBase_sptr SystemBase();

			template<class T>
			T& System();

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
		sys::cBase_sptr cECS::SystemBase()
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
		T& cECS::System()
		{
			return *std::dynamic_pointer_cast<T>(SystemBase<T>());
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

		template<class T>
		T * cECS::InstantiateComponent(cEntityWithData& ed)
		{
			auto ptr = ed->second.Component<T>();
			auto ptrnew = ed->second.AddComponent<T>();
			if (ptr)
				*ptrnew = *ptr;
			evt::cComponentAdded::Sig().emit(ed, cComponent<T>::StaticTypeIndex());
			return ptrnew;
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