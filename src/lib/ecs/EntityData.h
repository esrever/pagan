#pragma once

#include <vector>
#include <list>

#include "ecs-config.h"
#include "ComponentBase.h"
#include "Component.h"

namespace pgn
{
	namespace ecs
	{
		struct cEntityData
		{
		public:
			cEntityData() :mArchetype(nullptr){};
			void AddComponent(cComponentBase_sptr comp);
			void SetArchetype(const cEntityData& arch);

			template<class T>
			T * Component() const;

			template<class T>
			T * AddComponent();

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

		//--------------------------------------------------------
		template<class T>
		T * cEntityData::Component() const
		{
			if (cComponent<T>::StaticTypeIndex() >= mComponents.size())
				return nullptr;
			auto ptr = std::dynamic_pointer_cast<cComponent<T>>(mComponents[cComponent<T>::StaticTypeIndex()]);
			if (ptr)
				return &ptr->mData;
			else
				return nullptr;
		}

		template<class T>
		T * cEntityData::AddComponent()
		{
			auto ptr = cComponent<T>::Create();
			AddComponent(ptr);
			return &(dynamic_cast<cComponent<T> *>(ptr.get())->mData);
		}

	}

	DECL_SERIALIZE_INTERFACE(ecs::cEntityData);
	DECL_SERIALIZE_INTERFACE(std::vector<ecs::cComponentBase_sptr>);
}