#pragma once

#include <bitset>
#include <map>

#include "ecs_config.h"
#include "Entity.h"
#include "ComponentBase.h"

namespace pgn
{
	typedef std::pair<cEntityWptr, cComponentBaseWptr> EntityComponent;
	DECL_EVENT(ComponentAdded, EntityComponent);
	DECL_EVENT(RemoveComponent, EntityComponent);

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
