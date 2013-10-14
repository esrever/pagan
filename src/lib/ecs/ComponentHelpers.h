#pragma once

#include "ComponentBase.h"

namespace pgn
{
	class cComponentBase;
	class cEntity;
	//! Is this a bad,costly paradigm?
	bool ComponentBelongsToEntity(const cComponentBase& zComponent, const cEntity& zEntity);

	//! Gets a component of an (entity,type) pair
	cComponentBaseSptr GetComponent( const cEntity& zEntity, const unsigned short zTypeIndex);
}