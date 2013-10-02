#include "ComponentQuery.h"

#include <algorithm>

#include "EntityMgr.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cComponentQuery::cComponentQuery(const component_mask_type& zMask)
	:mMask(zMask)
	{
		// TODO: fetch all components that match the mask, from cEntityMgr
		assert(false);
		//use std::vector<int>::iterator it = std::find_if (myvector.begin(), myvector.end(), IsOdd); bool IsOdd (int i) {return ((i%2)==1);}
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
		auto e = *zData.data.lock();
		auto ored = ECS.GetComponents(e).Mask() & mMask;
		if( ored.count() == mMask.count())
			mEntitiesWithComponents.insert(e);

	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cDestroyEntityEventData& zData)
	{
		mEntitiesWithComponents.erase(*zData.data.lock());
	}
}