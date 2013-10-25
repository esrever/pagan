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
				mEntities.insert(x.first);
		}
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cComponentAddedEventData& zData)
	{
		const auto& e = std::get<0>(zData.data)->first;
		if( is_subset(ECS.mEntityMgr->GetComponents(e).Mask(), mMask))
			mEntities.insert(e);
	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cRemoveComponentEventData& zData)
	{
		const auto& e = std::get<0>(zData.data)->first;
		if( mMask[std::get<1>(zData.data)])
			mEntities.erase(e);	
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cEntityCreatedEventData& zData)
	{
		const auto& e = std::get<0>(zData.data);
		auto& ecs = ECS;
		if( is_subset(ecs.mEntityMgr->GetComponents(e).Mask(), mMask) )
			mEntities.insert(e);

	}
	//------------------------------------------------------------------------------
	void cComponentQuery::Receive(const cDestroyEntityEventData& zData)
	{
		mEntities.erase(std::get<0>(zData.data));
	}

	//------------------------------------------------------------------------------
	bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub)
	{
		return (zAll & zSub) == zSub;
	}
}