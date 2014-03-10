#pragma once

#include <vector>
#include <list>

#include "ecs-config.h"
#include "ComponentBase.h"
#include "Component.h"

namespace pgn
{
	struct cEntityData
	{
	public:
		cEntityData() :mArchetype(nullptr){};
		void AddComponent(cComponentBase_sptr comp);
		void SetArchetype(const cEntityData& arch);

		template<class T>
		T * Component();

	public:
		std::string							mName;
		//! List of component pointers
		std::vector<cComponentBase_sptr>	mComponents;
		//! Mask of supported components
		cComponentMask						mSupportMask;
		//! Mask of shared/read-only components
		cComponentMask						mSharedMask;
		//! Archetype
		const cEntityData *					mArchetype;
		//! List of tags
		std::set<std::string>				mTags;
	};

	DECL_SERIALIZE_INTERFACE(cEntityData);

	//--------------------------------------------------------
	template<class T>
	T * cEntityData::Component()
	{
		return std::dynamic_pointer_cast<T>(mComponents[cComponent<T>::StaticTypeIndex()]).get();
	}
}