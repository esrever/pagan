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

	//----------------------------------------------------------------
	void cEntityMgr::AddComponent(cEntityPtr zEntity, std::shared_ptr<cComponentBase> zComponent)
	{
		mComponents[zEntity.lock()].insert(zComponent);
		// TODO: EMIT_EVENT(ComponentAdded, )
	}

	//----------------------------------------------------------------
	void cEntityMgr::RemoveComponent(cEntityPtr zEntity, std::weak_ptr<cComponentBase> zComponent)
	{
		// TODO: EMIT_EVENT(RemoveComponent, )
		auto i = mComponents.find(zEntity.lock());
		assert(i !=mComponents.end());
		i->second.erase(zComponent.lock());
	}

	//----------------------------------------------------------------
	unsigned short cEntityMgr::AddComponentType( const std::type_index& zTi)
	{
		for(size_t i=0;i<mComponentTypeIds.size();++i)
		{
			if(zTi == mComponentTypeIds[i])
				return i;
		}
		mComponentTypeIds.push_back(zTi);
		return mComponentTypeIds.size() - 1;
	}

}