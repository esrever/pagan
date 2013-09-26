#include "EntityMgr.h"

namespace pgn
{
	//----------------------------------------------------------------
	cEntityPtr cEntityMgr::Create( cEntityPtr zEntity)
	{
		auto e = std::shared_ptr<cEntity>(new cEntity());
		mEntities.insert(e);
		EMIT_EVENT(EntityCreated,e);
		return e;
	}

	//----------------------------------------------------------------
	cEntityPtr cEntityMgr::Create( const component_mask_type& zMask, 
								cEntityPtr zEntity)
	{
		auto e = Create(zEntity);
		auto xored = zMask ^ mComponentMasks[zEntity];
		// TODO: create the components from the set bits
		return e;
	}

	void cEntityMgr::Destroy(cEntityPtr zEntity)
	{
		EMIT_EVENT(DestroyEntity, zEntity);
		// and finally erase it
		mEntities.erase(zEntity.lock());
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
		// unset bits
		mComponentMasks.erase(zData.data);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Receive( const cComponentMaskModifiedEventData& zData)
	{
		auto i = mComponentMasks.find(zData.data);
		assert(i != mComponentMasks.end()); // We just got the message! 
		if(i->second.none())
			Destroy(zData.data);
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