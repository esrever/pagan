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
		DECL_MAP_MEMBER_R(std::string, cEntity, TagsToEntities);
		DECL_MAP_MEMBER_R(std::string, cQueryFunc, Queries);

	public:
		typedef std::map<cEntity, cEntityData>::const_iterator  cEntityWithDataConst;
		typedef std::map<cEntity, cEntityData>::iterator        cEntityWithData;

		void Tag(const std::string&, const cEntityWithData);
		void Untag(const std::string&, const cEntityWithData);

		template<class T>
		void RegisterComponent();

	protected:

	private:
		std::vector<ComponentCreatorFunction> mComponentCreators;
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