#include "EntityMgr.h"

namespace pgn
{
	cEntitySptr cEntityMgr::Create() 
	{ 
		auto e = cEntity();
		auto it = mEntityComponents.insert(std::pair<cEntity,cEntityComponents>(e,cEntityComponents()));
		auto eptr = std::make_shared<cEntity>( it.first->first);
		EMIT_EVENT(EntityCreated,eptr);
		return eptr;
	}
	/*
	//----------------------------------------------------------------
	cEntityWptr cEntityMgr::Create( cEntityWptr zEntity)
	{
		auto e = std::shared_ptr<cEntity>(new cEntity());
		mEntities.insert(e);
		EMIT_EVENT(EntityCreated,e);
		return e;
	}

	//----------------------------------------------------------------
	cEntityWptr cEntityMgr::Create( const component_mask_type& zMask, 
								cEntityWptr zEntity)
	{
		auto e = Create(zEntity);
		auto xored = zMask ^ mComponentMasks[zEntity];
		// TODO: create the components from the set bits
		return e;
	}
	*/

	void cEntityMgr::Destroy(cEntityWptr zEntity)
	{
		EMIT_EVENT(DestroyEntity, zEntity);
		// and finally erase it
		mEntityComponents.erase(*zEntity.lock().get());
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
	void cEntityMgr::Tag(cEntityWptr zEntity, const std::string& zTag)
	{
		mTaggedEntities[zTag].insert(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntityWptr zEntity, const std::string& zTag)
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
	void cEntityMgr::Untag(cEntityWptr zEntity)
	{
		for(auto i : mTaggedEntities)
			i.second.erase(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::AddComponentPtr(cEntityWptr zEntity, cComponentBaseSptr zComponent)
	{
		auto i = mEntityComponents.find(*zEntity.lock().get());
		assert(i !=mEntityComponents.end());
		i->second.AddComponent(zComponent);
		auto evtd = std::pair<cEntityWptr,cComponentBaseWptr>(zEntity,zComponent);
		EMIT_EVENT(ComponentAdded, evtd);
	}

	//----------------------------------------------------------------
	void cEntityMgr::RemoveComponentPtr(cEntityWptr zEntity, cComponentBaseWptr zComponent)
	{
		auto evtd = std::pair<cEntityWptr,cComponentBaseWptr>(zEntity,zComponent);
		EMIT_EVENT(RemoveComponent, evtd);
		auto i = mEntityComponents.find(*zEntity.lock().get());
		assert(i !=mEntityComponents.end());
		i->second.RemoveComponent(zComponent);
		if(i->second.Mask().none())
			Destroy(zEntity);
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