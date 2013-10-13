#include "ComponentQuery.h"

#include <algorithm>

#include "ecs.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cComponentQuery::cComponentQuery(const component_mask_type& zMask)
	:mMask(zMask)
	{
		// Fetch all components that match the mask, from cEntityMgr
		for(auto x : ECS.mEntityMgr->GetComponents() )
		{
			if ( is_subset( x.second.Mask(), mMask))
				mEntitiesWithComponents.insert(x.first);
		}
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cComponentAddedEventData& zData)
	{
		const auto& e = *std::get<0>(zData.data).lock();
		if( is_subset(ECS.mEntityMgr->GetComponents(e).Mask(), mMask))
			mEntitiesWithComponents.insert(e);
	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cRemoveComponentEventData& zData)
	{
		const auto& e = *std::get<0>(zData.data).lock();
		if( mMask[std::get<1>(zData.data).lock()->TypeIndex()])
			mEntitiesWithComponents.erase(e);	
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cEntityCreatedEventData& zData)
	{
		const auto& e = *std::get<0>(zData.data).lock();
		auto& ecs = ECS;
		if( is_subset(ecs.mEntityMgr->GetComponents(e).Mask(), mMask) )
			mEntitiesWithComponents.insert(e);

	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cDestroyEntityEventData& zData)
	{
		mEntitiesWithComponents.erase(*std::get<0>(zData.data).lock());
	}

	//------------------------------------------------------------------------------
	bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub)
	{
		return (zAll & zSub) == zSub;
	}
}