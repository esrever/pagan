#include "EntityMgr.h"

#include <core/util/jsonutils.h>
#include "ecs.h"

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
		cEntityTaggedEventData::emit(std::make_tuple(zEntity,zTag));
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntity zEntity, const std::string& zTag)
	{
		// Look for the tag
		auto i1 = mTaggedEntities.find(zTag);
		if(i1 != mTaggedEntities.end())
		{
			cEntityUntagEventData::emit(std::make_tuple(zEntity,zTag));
			i1->second.erase(zEntity);
		}
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(const std::string& zTag)
	{
		// Look for the tag
		auto i1 = mTaggedEntities.find(zTag);
		if(i1 != mTaggedEntities.end())
		{
			cUntagEventData::emit(std::make_tuple(zTag));
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
		mComponentCreators.push_back(component_creator_fun());

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

	//----------------------------------------------------------------
	void cEntityMgr::ImportArchetypes(const rapidjson::Document * zDoc)
	{
		if (!zDoc) return;
		if(zDoc->IsArray())
			for (auto itr = zDoc->Begin(); itr != zDoc->End(); ++itr) 
			{
				// get an archetype object
				const auto& arch = *itr;
				assert(arch.IsObject());

				std::string name; 
				if( !pgn::from_json( name, arch["name"])) break;
				// Look for the inherited values from mArchetypes
				std::vector<std::string> inherited,components,tags;
				read_json_vector(inherited, arch["inherits"]);
				read_json_vector(components, arch["components"]);
				read_json_vector(tags, arch["tags"]);

				cArchetype archetype;
				archetype.mName = name;
				archetype.mTags = tags;
				for( const auto& x : inherited)
				{
					auto iter = mArchetypes.find(x);
					if(iter != mArchetypes.end())
						archetype.mMask |= iter->second.mMask;
				}
				for( const auto& x : components)
				{
					auto idx = GetComponentTypeIndex(x);
					if(idx != 0xFFFFFFFF)
						archetype.mMask.set(idx);
				}
				
				mArchetypes[name] = archetype;
			}
	}

	//----------------------------------------------------------------
	void cEntityMgr::ImportInstances(const rapidjson::Document * zDoc, bool zIsExemplar)
	{
		if (!zDoc) return;
		if(zDoc->IsArray())
		{
			for (auto itr = zDoc->Begin(); itr != zDoc->End(); ++itr) 
			{
				// get an instance
				const auto& inst = *itr;
				assert(inst.IsObject());
				std::string archname, instname;
				std::vector<std::string> tags;
				std::vector<std::shared_ptr<cComponentBase>> componentPtrs;
				for (auto mitr = inst.MemberBegin(); mitr != inst.MemberEnd(); ++mitr) 
				{
					if( strcmp( mitr->name.GetString(),"archetype") == 0)
						archname = mitr->value.GetString();
					else if ( strcmp( mitr->name.GetString(),"name") == 0)
						instname = mitr->value.GetString();
					else if ( strcmp( mitr->name.GetString(),"tags") == 0)
					{
						read_json_vector(tags,mitr->value);
					}
					else
					{
						const std::string componentType = mitr->name.GetString();
						auto sptr = CreateComponent(componentType);
						if(sptr)
						{
							sptr->from_json(mitr->value);
							componentPtrs.push_back(sptr);
						}
					}
				}

				// Create the entity
				if( (archname != "") && (instname != "") && (!componentPtrs.empty()))
				{
					auto e = InstantiateArchetype(archname);

					for( auto c : componentPtrs)
						AddComponentPtr(e,c);
					for( auto t : tags)
						Tag(e,t);
					if (zIsExemplar)
					{
						mExemplars[instname] = e;
						Tag(e,"Exemplar");
					}
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	bool cEntityMgr::from_json(const rapidjson::Value& zRoot)
	{
		const auto&  qobj = zRoot["ArchetypesFile"];
		std::vector<std::string> fnames;
		read_json_vector(fnames, zRoot["ArchetypesFile"]);
		for(auto s : fnames)
		{
			auto pdoc = file_to_json(ECS.GetDataPath() + s);
			ImportArchetypes(pdoc.get());
		}
		read_json_vector(fnames, zRoot["ExemplarsFile"]);
		for(auto s : fnames)
		{
			auto pdoc = file_to_json(ECS.GetDataPath() + s);
			ImportInstances(pdoc.get(),true);
		}
		read_json_vector(fnames, zRoot["InstancesFile"]);
		for(auto s : fnames)
		{
			auto pdoc = file_to_json(ECS.GetDataPath() + s);
			ImportInstances(pdoc.get(),false);
		}		
        return true;
	}

	//-------------------------------------------------------------------------------------------------
	std::shared_ptr<cComponentBase> cEntityMgr::CreateComponent(const std::string& zName) const
	{
		auto it = mComponentTypeNamesToIds.find(zName);
		if(it == mComponentTypeNamesToIds.end())
			return nullptr;
		else
			return CreateComponent(it->second);
	}

	//-------------------------------------------------------------------------------------------------
	std::shared_ptr<cComponentBase> cEntityMgr::CreateComponent(size_t zIdx) const
	{
		return mComponentCreators.at(zIdx)();
	}

	//-------------------------------------------------------------------------------------------------
	cEntity cEntityMgr::InstantiateArchetype(const std::string& zArchName)
	{
		auto e = Create();
		auto itArch = mArchetypes.find(zArchName);
		if(itArch != mArchetypes.end())
		{
			const auto& arch = itArch->second;
			// Add tags
			for( auto t : arch.mTags)
				Tag(e,t);
			// Add components
			for (size_t i = 0;i < arch.mMask.size();++i)
				if(arch.mMask.test(i))
				{
					auto compo = CreateComponent( i);
					AddComponentPtr(e,compo);
				} 
		}
		return e;
	}

	//-------------------------------------------------------------------------------------------------
	cEntity cEntityMgr::InstantiateExemplar(const std::string& zExemplarName)
	{
		auto e = Create();
		auto itEx = mExemplars.find(zExemplarName);
		// Copy components
		const auto& ec = GetComponents(itEx->second);
		const auto& cset = ec.Components();
		for (const auto& x : cset)
		{
			if(x)
			{
				auto sptr = CreateComponent( x->TypeIndex());
				*sptr = *x;
				AddComponentPtr(e, sptr);
			}
		}

		// Copy tags
		for (const auto& it : TaggedEntities())
		{
			if(it.second.find(itEx->second) != it.second.end())
				Tag(e,it.first);
		}

		// Remove exemplar tag
		Untag(e,"Exemplar");
		return e;
	}
}