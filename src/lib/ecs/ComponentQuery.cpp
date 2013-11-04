#include "ComponentQuery.h"

#include <algorithm>

#include "ecs.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cComponentQuery::cComponentQuery(const component_mask_type& zMask)
	:mMask(zMask)
	, mOnEntityCreated(Simple::slot(this, &cComponentQuery::OnEntityCreated))
	, mOnEntityDestroy(Simple::slot(this, &cComponentQuery::OnEntityDestroy))
	, mOnComponentAdded(Simple::slot(this, &cComponentQuery::OnComponentAdded))
	, mOnComponentRemove(Simple::slot(this, &cComponentQuery::OnComponentRemove))
	{
		// Fetch all components that match the mask, from cEntityMgr
		for(auto x : ECS.mEntityMgr->GetComponents() )
		{
			if ( is_subset( x.second.Mask(), mMask))
				mEntities.insert(x.first);
		}
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::OnComponentAdded(cEntityWithComponents ec, unsigned short )
	{
		const auto& e = ec->first;
		if( is_subset(ECS.mEntityMgr->GetComponents(e).Mask(), mMask))
			mEntities.insert(e);
	}
	//------------------------------------------------------------------------------
	void cComponentQuery::OnComponentRemove(cEntityWithComponents ec, unsigned short cid)
	{
		const auto& e = ec->first;
		if( mMask[cid])
			mEntities.erase(e);	
	}

	//------------------------------------------------------------------------------
	void cComponentQuery::OnEntityCreated(cEntity e)
	{
		auto& ecs = ECS;
		if( is_subset(ecs.mEntityMgr->GetComponents(e).Mask(), mMask) )
			mEntities.insert(e);

	}
	//------------------------------------------------------------------------------
	void cComponentQuery::OnEntityDestroy(cEntity e)
	{
		mEntities.erase(e);
	}

	//------------------------------------------------------------------------------
	bool is_subset(const component_mask_type& zAll, const component_mask_type& zSub)
	{
		return (zAll & zSub) == zSub;
	}
}