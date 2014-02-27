#pragma once

#include "ecs_config.h"
#include "Component.h"

namespace pgn
{
	class cComponentBase;
	//! Is this a bad,costly paradigm?
	bool ComponentBelongsToEntity(const cComponentBase& zComponent, const cEntity& zEntity);

	//! Gets a ComponentBase from an (entity, typeIndex) pair
	cComponentBaseSptr GetComponent( const cEntity& zEntity, const unsigned short zTypeIndex);
	//! Gets a Component<T> from an (entity, T) pair
	template<class T> 
	bool GetComponent(const cEntity& zEntity, std::shared_ptr< cComponent<typename T> >& ptr);

	//-------------------------------------------------------------------------------------------------
	template<class T> 
	bool GetComponent(const cEntity& zEntity, std::shared_ptr< cComponent<typename T> >& ptr)
	{
		const auto& compos = ECS.mEntityMgr->GetComponents(zEntity);
		compos.GetComponent(ptr);
		return ptr != nullptr;
	}
}