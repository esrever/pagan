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
		T * Component() const;

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
		//! List of tags, unique or not
		std::set<std::string>				mTags;
		std::set<std::string>				mTagus;
	};

	DECL_SERIALIZE_INTERFACE(cEntityData);
	DECL_SERIALIZE_INTERFACE(std::vector<cComponentBase_sptr>);

	//--------------------------------------------------------
	template<class T>
	T * cEntityData::Component() const
	{
		auto ptr = std::dynamic_pointer_cast<cComponent<T>>(mComponents[cComponent<T>::StaticTypeIndex()]);
		if (ptr)
			return &ptr->mData;
		else
			return nullptr;
	}
}