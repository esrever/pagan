#pragma once

#include <bitset>
#include <set>

#include "ecs_config.h"
#include "Entity.h"
#include "ComponentBase.h"

namespace pgn
{
	class cEntityComponents
	{
		typedef std::set<cComponentBaseSptr> ComponentSet;

	public:
		void AddComponent(cComponentBaseSptr zCompo);
		void RemoveComponent(cComponentBaseWptr zCompo);

	private:
		//! All components
		ComponentSet mComponents;
		std::bitset<MAX_COMPONENTS> mMask;
	};

	//! Comparison criterion
	bool operator < (const cComponentBaseSptr& lhs, const cComponentBaseSptr& rhs)
	{
		return lhs.get()->TypeIndex() < rhs.get()->TypeIndex();
	}
}