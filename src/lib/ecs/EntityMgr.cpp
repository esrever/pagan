#include "EntityMgr.h"

#include <iostream>

namespace pgn
{
	//----------------------------------------------------------------
	cEntity cEntityMgr::Create() 
	{ 
		const auto& e = mEntityIdGen.New();
		auto it = mEntityComponents.insert(std::pair<cEntity,cEntityComponents>(e,cEntityComponents()));
		cEntityCreatedEventData::emit(std::make_tuple(e));
		return e;
	}

	//----------------------------------------------------------------
	void cEntityMgr::Destroy(cEntity zEntity)
	{
		cDestroyEntityEventData::emit(std::make_tuple(zEntity));
		// and finally erase it
		mEntityComponents.erase(zEntity);
		mEntityIdGen.Destroy(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Receive( const cEntityCreatedEventData& zData)
	{
		
	}
	
	//----------------------------------------------------------------
	void cEntityMgr::Receive( const cDestroyEntityEventData& zData)
	{
		// untag from all groups
		Untag(std::get<0>(zData.data));
	}

	//----------------------------------------------------------------
	void cEntityMgr::Tag(cEntity zEntity, const std::string& zTag)
	{
		mTaggedEntities[zTag].insert(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntity zEntity, const std::string& zTag)
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
		{
			i1->second.clear();
			mTaggedEntities.erase(i1);
		}
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntity zEntity)
	{
		for(auto i : mTaggedEntities)
			i.second.erase(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::AddComponentPtr(cEntity zEntity, cComponentBaseSptr zComponent)
	{
		const auto& e = zEntity;
		auto i = mEntityComponents.find(e);
		assert(i !=mEntityComponents.end());
		i->second.AddComponent(zComponent);
		cComponentAddedEventData::emit(std::make_tuple(i,zComponent->TypeIndex()));
	}

	//----------------------------------------------------------------
	void cEntityMgr::RemoveComponentPtr(cEntity zEntity, cComponentBaseWptr zComponent)
	{
		auto i = mEntityComponents.find(zEntity);
		assert(i !=mEntityComponents.end());
		cRemoveComponentEventData::emit(std::make_tuple(i,zComponent.lock()->TypeIndex()));
		i->second.RemoveComponent(zComponent.lock()->TypeIndex());
		//! No components -> destroy entity
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

		std::vector<std::string> result;
		pystring::split(zTi.name(), result, " ");
		mComponentTypeNamesToIds[ result.back() ] = mComponentTypeIds.size() - 1;
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

	//----------------------------------------------------------------
	size_t cEntityMgr::GetComponentTypeIndex( const std::string& zName) const
	{
		auto f = mComponentTypeNamesToIds.find(zName);
		return f != mComponentTypeNamesToIds.end() ? f->second : 0xFFFFFFFF;
	}
}