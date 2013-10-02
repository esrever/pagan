#include "EntityMgr.h"

#include <iostream>

namespace pgn
{
	cEntityMgr::cEntityMgr()
	{
		std::ios_base::Init();
		mLog.SetName("ECS");
		mLog.SetChannel(&std::cout);
	}

	//----------------------------------------------------------------
	cEntitySptr cEntityMgr::Create() 
	{ 
		auto e = cEntity();
		auto it = mEntityComponents.insert(std::pair<cEntity,cEntityComponents>(e,cEntityComponents()));
		auto eptr = std::make_shared<cEntity>( it.first->first);
		cEntityWptr ewptr = eptr;
		cEntityCreatedEventData::emit(ewptr);
		return eptr;
	}

	//----------------------------------------------------------------
	void cEntityMgr::Destroy(cEntityWptr zEntity)
	{
		cDestroyEntityEventData::emit(zEntity);
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
		mTaggedEntities[zTag].insert(*zEntity.lock());
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntityWptr zEntity, const std::string& zTag)
	{
		// Look for the tag
		auto i1 = mTaggedEntities.find(zTag);
		if(i1 != mTaggedEntities.end())
			i1->second.erase(*zEntity.lock());
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(const std::string& zTag)
	{
		// Look for the tag
		auto i1 = mTaggedEntities.find(zTag);
		if(i1 != mTaggedEntities.end())
		{
			i1->second.clear();
			mTaggedEntities.erase(i1);
		}
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntityWptr zEntity)
	{
		for(auto i : mTaggedEntities)
			i.second.erase(*zEntity.lock());
	}

	//----------------------------------------------------------------
	void cEntityMgr::AddComponentPtr(cEntityWptr zEntity, cComponentBaseSptr zComponent)
	{
		auto i = mEntityComponents.find(*zEntity.lock().get());
		assert(i !=mEntityComponents.end());
		i->second.AddComponent(zComponent);
		auto evtd = std::pair<cEntityWptr,cComponentBaseWptr>(zEntity,zComponent);
		//EMIT_EVENT(ComponentAdded, evtd);
		//EMIT_EVENT(ComponentAdded, evtd);
		cComponentAddedEventData::emit(evtd);
	}

	//----------------------------------------------------------------
	void cEntityMgr::RemoveComponentPtr(cEntityWptr zEntity, cComponentBaseWptr zComponent)
	{
		auto evtd = std::pair<cEntityWptr,cComponentBaseWptr>(zEntity,zComponent);
		cRemoveComponentEventData::emit(evtd);
		auto i = mEntityComponents.find(*zEntity.lock());
		assert(i !=mEntityComponents.end());
		i->second.RemoveComponent(zComponent.lock()->TypeIndex());
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
	
	//----------------------------------------------------------------
	const cEntityComponents& cEntityMgr::GetComponents(const cEntity& zEntity) const
	{
		auto f = mEntityComponents.find(zEntity);
		assert(f != mEntityComponents.end());
		return f->second;
	}

	//----------------------------------------------------------------
	const std::map<cEntity, cEntityComponents>& cEntityMgr::GetComponents() const
	{
		return mEntityComponents;
	}

}