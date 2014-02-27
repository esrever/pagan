#pragma once

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "ecs-config.h"
#include "component.h"


#define DECL_MAP_MEMBER_R(A,B, N) \
	public: const std::map< A , B > & N () const { return m##N ; }\
	private: std::map< A , B > m##N ; 

namespace pgn
{
	typedef std::function< std::shared_ptr<cComponentBase>() > ComponentCreatorFunction;
	struct cEntityData;

	class cECS
	{
		DECL_MAP_MEMBER_R(cEntity, cEntityData, EntitiesToData);
		DECL_MAP_MEMBER_R(std::string, cEntityData, Archetypes);
		DECL_MAP_MEMBER_R(std::string, std::set<cEntity>, TagsToEntities);
		DECL_MAP_MEMBER_R(std::string, cQueryFunc, Queries);

		SUPPORT_DERIVED(cECS);

	public:
		typedef std::map<cEntity, cEntityData>::const_iterator  cEntityWithDataConst;
		typedef std::map<cEntity, cEntityData>::iterator        cEntityWithData;

	public:
		void Tag(const std::string&, cEntityWithData);
		void Untag(const std::string&, cEntityWithData);

	protected:
		template<class T>
		void RegisterComponent();

	private:
		unsigned short AddComponentType(const std::type_index& zTi);

	private:
		std::vector<ComponentCreatorFunction> mComponentCreators;
		std::vector< std::type_index>		  mComponentTypeIds;
		std::map< std::string, size_t >		  mComponentTypeNamesToIds;
	};

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