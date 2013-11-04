#include "TagQuery.h"

#include <algorithm>

#include "ecs.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cTagQuery::cTagQuery(const std::string& zTag)
	:mOnEntityTagged(Simple::slot(this, &cTagQuery::OnEntityTagged))
	,mOnEntityUntag(Simple::slot(this, &cTagQuery::OnEntityUntag))
	{
		mTags.push_back(zTag);
		Init();
	}

	//------------------------------------------------------------------------------
	cTagQuery::cTagQuery(const std::vector<std::string>& zTags)
	:mOnEntityTagged(Simple::slot(this, &cTagQuery::OnEntityTagged))
	, mOnEntityUntag(Simple::slot(this, &cTagQuery::OnEntityUntag))
	{
		mTags = zTags;
		Init();
	}

	//------------------------------------------------------------------------------
	void cTagQuery::Init()
	{
		const auto& te = ECS.mEntityMgr->TaggedEntities();
		for(size_t i=0;i<mTags.size();++i)
		{
			auto it = te.find(mTags.at(i));
			if(it != te.end())
				mEntities.insert(it->second.begin(), it->second.end());
		}
	}

	//------------------------------------------------------------------------------
	void cTagQuery::OnEntityTagged(cEntity e, const std::string& zTag)
	{
		if (std::find(mTags.begin(), mTags.end(), zTag) != mTags.end())
			mEntities.insert(e);
	}
	//------------------------------------------------------------------------------
	void cTagQuery::OnEntityUntag(cEntity e, const std::string& zTag)
	{
		if (std::find(mTags.begin(), mTags.end(), zTag) != mTags.end())
			mEntities.erase(e);
	}

}