#include "ComponentQuery.h"

#include <algorithm>

#include "EntityMgr.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cComponentQuery::cComponentQuery(const component_mask_type& zMask)
	:mMask(zMask)
	{
		// Fetch all components that match the mask, from cEntityMgr
		for(auto x : ECS.GetComponents() )
		{
			if ( is_subset( x.second.Mask(), mMask))
				mEntitiesWithComponents.insert(x.first);
		}
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cComponentAddedEventData& zData)
	{
		auto e = *zData.data.first.lock();
		if( is_subset(ECS.GetComponents(e).Mask(), mMask))
			mEntitiesWithComponents.insert(e);
	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cRemoveComponentEventData& zData)
	{
		auto e = *zData.data.first.lock();
		if( mMask[zData.data.second.lock()->TypeIndex()])
			mEntitiesWithComponents.erase(e);	
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cEntityCreatedEventData& zData)
	{
		auto e = *zData.data.lock();
		if( is_subset(ECS.GetComponents(e).Mask(), mMask) )
			mEntitiesWithComponents.insert(e);

	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cDestroyEntityEventData& zData)
	{
		mEntitiesWithComponents.erase(*zData.data.lock());
	}

	//------------------------------------------------------------------------------
	bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub)
	{
		return (zAll & zSub) == zSub;
	}
}