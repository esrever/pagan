#pragma once

#include <bitset>
#include <map>

#include "ecs_config.h"
#include "Entity.h"
#include "ComponentBase.h"

namespace pgn
{
	class cEntityComponents
	{
		typedef std::vector<cComponentBaseSptr> ComponentSet;

	public:
		cEntityComponents();
		void AddComponent(cComponentBaseSptr zCompo);
		void RemoveComponent(unsigned short zTypeId);

		const component_mask_type& Mask() const {return mMask;}
		const ComponentSet& Components() const {return mComponents;}

	private:
		//! All components
		ComponentSet mComponents;
		component_mask_type mMask;
	};

	typedef std::map<cEntity, cEntityComponents>::const_iterator cEntityWithComponents;
	DECL_EVENT2(ComponentAdded, cEntityWithComponents, unsigned short );
	DECL_EVENT2(RemoveComponent, cEntityWithComponents, unsigned short );
}
