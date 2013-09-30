#include "ComponentQuery.h"
#include "EntityMgr.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cComponentQuery::cComponentQuery(const component_mask_type& zMask)
	{
		// TODO: fetch all components that match the mask, from cEntityMgr
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cComponentAddedEventData& zData)
	{
		//TODO: if mask | thismask, tryAddEntity, else, tryRemoveEntity
	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cRemoveComponentEventData& zData)
	{
		//TODO: if mask | thismask, tryAddEntity, else, tryRemoveEntity
	}
}