#pragma once

#include "MoveAdjacent.h"

#include <ecs/ecs.h>
#include <ecs/EntityMgr.h>
#include <ecs/EntityComponents.h>

#include <rl/components/Location.h>

namespace pgn
{
	void MoveAdjacent(cEntity e, int x, int y)
	{
		// TODO: Change the entity's location, if possible. Emit messages to guide text log, e.g. "{e.name} bumped onto ${e2.name}"
		auto& compo = ECS.mEntityMgr->GetComponents(e);

		std::shared_ptr<cComponent<cLocation>> locPtr;
		compo.GetComponent(locPtr);
		
		// TODO: Get current level. Should be cheaply available
		locPtr->mData.mCoords;

		// TODO: Query coords for tile type
		// TODO: Query coords for other entities
		// TODO: If tile type has IsObstacle && its true, we're blocked. 
		// TODO: If another entity is on the tile that has "Pushable" component, do Push.
			// TODO: If push is successful, move to the spot, emit message
	}
}