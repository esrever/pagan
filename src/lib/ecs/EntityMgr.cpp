#include "EntityMgr.h"

namespace pgn
{
	//----------------------------------------------------------------
	cEntity cEntityMgr::Create( const cEntity& zEntity)
	{
		// TODO: new globally unique entity
		cEntity e;

		// TODO: copy mask & associated data from existing entity
		
		return e;
	}

	//----------------------------------------------------------------
	cEntity cEntityMgr::Create( const cEntity::component_mask_type& zMask, 
								const cEntity& zEntity)
	{
		cEntity e = Create(zEntity);
		
		// TODO: xor the mask and create/init the rest data
		return e;
	}

	//----------------------------------------------------------------
	void cEntityMgr::Receive( const cEntityCreatedEventData& zData)
	{
		
	}
	
	//----------------------------------------------------------------
	void cEntityMgr::Receive( const cDestroyEntityEventData& zData)
	{
		// untag from all groups
		Untag(zData.data);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Tag(const cEntity& zEntity, const std::string& zTag)
	{
		mTaggedEntities[zTag].insert(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(const cEntity& zEntity, const std::string& zTag)
	{
		// Look for the tag
		auto i1 = mTaggedEntities.find(zTag);
		if(i1 != mTaggedEntities.end())
			i1->second.erase(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(const std::string& zTag)
	{
		// Look for the tag
		auto i1 = mTaggedEntities.find(zTag);
		if(i1 != mTaggedEntities.end())
			i1->second.clear();
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(const cEntity& zEntity)
	{
		for(auto i : mTaggedEntities)
			i.second.erase(zEntity);
	}
}