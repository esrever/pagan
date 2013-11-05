#include "QueryExpression.h"

#include <algorithm>

#include "ecs.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cQueryExpression::cQueryExpression()
		: mOnEntityCreated(Simple::slot(this, &cQueryExpression::OnEntityCreated))
		, mOnEntityDestroy(Simple::slot(this, &cQueryExpression::OnEntityDestroy))
		, mOnComponentAdded(Simple::slot(this, &cQueryExpression::OnComponentAdded))
		, mOnComponentRemove(Simple::slot(this, &cQueryExpression::OnComponentRemove))
		, mOnEntityTagged(Simple::slot(this, &cQueryExpression::OnEntityTagged))
		, mOnEntityUntag(Simple::slot(this, &cQueryExpression::OnEntityUntag))
		, mOnTagRemove(Simple::slot(this, &cQueryExpression::OnTagRemove))
	{
		// Fetch all components that match the mask, from cEntityMgr
		for (auto x : ECS.mEntityMgr->GetComponents())
		{
			if (is_subset(x.second.Mask(), mMask))
				mEntities.insert(x.first);
		}
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnComponentAdded(cEntityWithComponents ec, unsigned short)
	{
		const auto& e = ec->first;
		if (is_subset(ECS.mEntityMgr->GetComponents(e).Mask(), mMask))
			mEntities.insert(e);
	}
	//------------------------------------------------------------------------------
	void cQueryExpression::OnComponentRemove(cEntityWithComponents ec, unsigned short cid)
	{
		const auto& e = ec->first;
		if (mMask[cid])
			mEntities.erase(e);
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityCreated(cEntity e)
	{
		auto& ecs = ECS;
		if (is_subset(ecs.mEntityMgr->GetComponents(e).Mask(), mMask))
			mEntities.insert(e);

	}
	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityDestroy(cEntity e)
	{
		mEntities.erase(e);
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityTagged(cEntity e, const std::string& zTag)
	{
		if (std::find(mTags.begin(), mTags.end(), zTag) != mTags.end())
			mEntities.insert(e);
	}
	//------------------------------------------------------------------------------
	void cQueryExpression::OnEntityUntag(cEntity e, const std::string& zTag)
	{
		if (std::find(mTags.begin(), mTags.end(), zTag) != mTags.end())
			mEntities.erase(e);
	}

	//------------------------------------------------------------------------------
	void cQueryExpression::OnTagRemove(const std::string& zTag)
	{
		// If the tag is in one of our list, what happens? issue a warning, as compos will forever be stripped!
		assert(false);
	}

	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------
	template<>
	bool from_json<cQueryExpression>(cQueryExpression& zObj, const rapidjson::Value& zRoot)
	{
		
	}
}