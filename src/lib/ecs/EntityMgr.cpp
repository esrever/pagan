#include "EntityMgr.h"

namespace pgn
{
	//----------------------------------------------------------------
	cEntityPtr cEntityMgr::Create( cEntityPtr zEntity)
	{
		// TODO: new globally unique entity
		cEntity e;

		// TODO: copy mask & associated data from existing entity
		
		return e.sptr();
	}

	//----------------------------------------------------------------
	cEntityPtr cEntityMgr::Create( const component_mask_type& zMask, 
								cEntityPtr zEntity)
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
		mComponentMasks.erase(zData.data);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Receive( const cComponentMaskModifiedEventData& zData)
	{
		auto i = mComponentMasks.find(zData.data);
		assert(i != mComponentMasks.end()); // We just got the message! 
		if(i->second.none())
			EMIT_EVENT(DestroyEntity, zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Tag(cEntityPtr zEntity, const std::string& zTag)
	{
		mTaggedEntities[zTag].insert(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntityPtr zEntity, const std::string& zTag)
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
	void cEntityMgr::Untag(cEntityPtr zEntity)
	{
		for(auto i : mTaggedEntities)
			i.second.erase(zEntity);
	}
}