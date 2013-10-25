#include "TagQuery.h"

#include <algorithm>

#include "ecs.h"

namespace pgn
{
	//------------------------------------------------------------------------------
	cTagQuery::cTagQuery(const std::string& zTag)
	{
		mTags.push_back(zTag);
		Init();
	}

	//------------------------------------------------------------------------------
	cTagQuery::cTagQuery(const std::vector<std::string>& zTags)
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
	void cTagQuery::Receive(const cEntityTaggedEventData& zData)
	{
		assert(false);
	}
	//------------------------------------------------------------------------------
	void cTagQuery::Receive(const cEntityUntagEventData& zData)
	{
		assert(false);
	}

	//------------------------------------------------------------------------------
	void cTagQuery::Receive(const cEntityCreatedEventData& zData)
	{
		assert(false);
	}
	//------------------------------------------------------------------------------
	void cTagQuery::Receive(const cDestroyEntityEventData& zData)
	{
		assert(false);
	}

}