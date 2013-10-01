#include "ComponentQuery.h"
#include "EntityMgr.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cComponentQuery::cComponentQuery(const component_mask_type& zMask)
	{
		// TODO: fetch all components that match the mask, from cEntityMgr
		assert(false);
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cComponentAddedEventData& zData)
	{
		//TODO: if mask | thismask, tryAddEntity, else, tryRemoveEntity
		assert(false);
	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cRemoveComponentEventData& zData)
	{
		//TODO: if mask | thismask, tryAddEntity, else, tryRemoveEntity
		assert(false);
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cEntityCreatedEventData& zData)
	{
		//TODO: search through the components of the entity and add it if it contains the mask
		assert(false);
	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cDestroyEntityEventData& zData)
	{
		//TODO: if entity exists here, remove it.
		assert(false);
	}
}