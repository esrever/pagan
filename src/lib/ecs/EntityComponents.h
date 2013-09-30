#pragma once

#include <bitset>
#include <set>

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
		typedef std::set<cComponentBaseSptr> ComponentSet;

	public:
		void AddComponent(cComponentBaseSptr zCompo);
		void RemoveComponent(cComponentBaseWptr zCompo);

		const component_mask_type& Mask() const {return mMask;}
		const ComponentSet& Components() const {return mComponents;}

	private:
		//! All components
		ComponentSet mComponents;
		component_mask_type mMask;
	};
}

//! Comparison criterion
bool std::less<pgn::cComponentBaseSptr>::operator ()(const pgn::cComponentBaseSptr& lhs, const pgn::cComponentBaseSptr& rhs) const
{
	return lhs.get()->TypeIndex() < rhs.get()->TypeIndex();
}