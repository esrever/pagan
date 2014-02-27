#pragma once

#include <vector>
#include <list>

#include "ecs-config.h"
#include "ComponentBase.h"

namespace pgn
{
	/*
		Notes: How do I differentiate between shared and non-shared data with archetypes?
		Scenario 1:
			Archetype has A,B
			Entity has A,B,C
			A,B are shared/read-only
		Scenario 2:
			Archetype has A,B
			Entity has A,B,C
			A is shared/read-only
			B is overriden
	*/
	struct cEntityData
	{
	public:
		cEntityData() :mArchetype(nullptr){};
		void AddComponent(cComponentBase_sptr comp);

	public:
		//! List of component pointers
		std::vector<cComponentBase_sptr> mComponents;
		//! Mask of supported components
		cComponentMask				 mSupportMask;
		//! Mask of shared/read-only components
		cComponentMask				 mSharedMask;
		//! Archetype
		const cEntityData *			 mArchetype;
		//! List of tags
		std::set<std::string>     mTags;
	};

	DECL_SERIALIZE_INTERFACE(cEntityData);
}