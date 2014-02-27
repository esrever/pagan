#include "ComponentHelpers.h"
#include "ecs.h"
#include "EntityMgr.h"

namespace pgn
{
	//--------------------------------------------------------------------------------
	bool ComponentBelongsToEntity(const cComponentBase& zComponent, const cEntity& zEntity)
	{
		// Is this a bad paradigm?
		assert(false);
		const auto& ecs = ECS;
		const auto& compos = ecs.mEntityMgr->GetEntityData(zEntity).mComponents;
		const auto& cit = compos.Components().at( zComponent.TypeIndex());
		//return (cit!=compos.Components().end()) && (*cit->second.get() == zComponent);
		return false;
	}

	//--------------------------------------------------------------------------------
	cComponentBaseSptr GetComponent( const cEntity& zEntity, const unsigned short zTypeIndex)
	{
		const auto& ecs = ECS;
		const auto& compos = ecs.mEntityMgr->GetEntityData(zEntity).mComponents;
		return compos.Components().at( zTypeIndex);
	}
}