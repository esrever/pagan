#pragma once

#include <bitset>
#include <map>

#include "ecs_config.h"
#include "Entity.h"
#include "ComponentBase.h"

namespace pgn
{
	DECL_EVENT2(ComponentAdded, cEntityWptr, cComponentBaseWptr);
	DECL_EVENT2(RemoveComponent, cEntityWptr, cComponentBaseWptr);

	class cEntityComponents
	{
		typedef std::map<unsigned short, cComponentBaseSptr> ComponentSet;

	public:
		void AddComponent(cComponentBaseSptr zCompo);
		void RemoveComponent(unsigned short zTypeId);

		const component_mask_type& Mask() const {return mMask;}
		const ComponentSet& Components() const {return mComponents;}

	private:
		//! All components
		ComponentSet mComponents;
		component_mask_type mMask;
	};
}
