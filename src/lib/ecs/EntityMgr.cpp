#include "EntityMgr.h"

#include <core/util/jsonutils.h>
#include "ecs.h"

#include <iostream>

namespace pgn
{
	cEntityMgr::cEntityMgr()
	:mOnEntityCreated(Simple::slot(this, &cEntityMgr::OnEntityCreated))
	,mOnEntityDestroy(Simple::slot(this, &cEntityMgr::OnEntityDestroy))
	{}
	//----------------------------------------------------------------
	cEntity cEntityMgr::Create() 
	{ 
		const auto& e = mEntityIdGen.New();
		auto it = mEntityData.insert(std::pair<cEntity,cEntityData>(e,cEntityData()));
		evt::cEntityCreated::mSig.emit(e);
		return e;
	}

	//----------------------------------------------------------------
	void cEntityMgr::Destroy(cEntity zEntity)
	{
		evt::cEntityDestroy::mSig.emit(zEntity);
		// and finally erase it
		mEntityData.erase(zEntity);
		mEntityIdGen.Destroy(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::OnEntityCreated(cEntity e)
	{
	}
	
	//----------------------------------------------------------------
	void cEntityMgr::OnEntityDestroy( cEntity e)
	{
		// untag from all groups
		Untag(e);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Tag(cEntity zEntity, const std::string& zTag)
	{
		if (zTag == "Player")
			mGlobals.mPC = zEntity;
		else if (zTag == "World")
			mGlobals.mWorld = zEntity;
		mTaggedEntities[zTag].insert(zEntity);
		evt::cEntityTagged::mSig.emit(zEntity, zTag);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntity zEntity, const std::string& zTag)
	{
		// Look for the tag
		auto i1 = mTaggedEntities.find(zTag);
		if(i1 != mTaggedEntities.end())
		{
			evt::cEntityUntag::mSig.emit(zEntity, zTag);
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
			evt::cTagRemove::mSig.emit(zTag);
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
		auto i = mEntityData.find(e);
		assert(i !=mEntityData.end());
		i->second.mComponents.AddComponent(zComponent);
		evt::cComponentAdded::mSig.emit(i, zComponent->TypeIndex());
	}

	//----------------------------------------------------------------
	void cEntityMgr::RemoveComponentPtr(cEntity zEntity, cComponentBaseWptr zComponent)
	{
		auto i = mEntityData.find(zEntity);
		assert(i !=mEntityData.end());
		evt::cComponentRemove::mSig.emit(i, zComponent.lock()->TypeIndex());
		i->second.mComponents.RemoveComponent(zComponent.lock()->TypeIndex());
		//! No components -> destroy entity
		if (i->second.mComponents.Mask().none())
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
	const cEntityData& cEntityMgr::GetEntityData(const cEntity& zEntity) const
	{
		auto f = mEntityData.find(zEntity);
		assert(f != mEntityData.end());
		return f->second;
	}

	//----------------------------------------------------------------
	const std::map<cEntity, cEntityData>& cEntityMgr::GetEntityData() const
	{
		return mEntityData;
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
					if (idx != 0xFFFFFFFF)
						archetype.mMask.set(idx);
					else
						ECS.mLog->Err(boost::str(boost::format("Component type %s is not registered") % x.c_str()));
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
						read_json_vector(tags,mitr->value);
					else
					{
						const std::string componentType = mitr->name.GetString();
						auto sptr = CreateComponent(componentType);
						if(sptr)
						{
							sptr->from_json(mitr->value);
							componentPtrs.push_back(sptr);
						}
						else
							ECS.mLog->Err(boost::str(boost::format("Component type %s is not registered") % componentType.c_str()));
					}
				}

				// Create the entity
				if( (archname != "") && (instname != "") && (!componentPtrs.empty()))
				{
					// Archetype to exemplar
					cExemplar exemplar = ArchetypeToExemplar(archname);
					exemplar.mName = instname;
					exemplar.mTags.insert(exemplar.mTags.end(), tags.begin(), tags.end());
					for (auto c : componentPtrs)
						exemplar.mComponents.AddComponent(c);

					mExemplars[exemplar.mName] = exemplar;
					if (!zIsExemplar)
						InstantiateExemplar(exemplar.mName);
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
	cExemplar cEntityMgr::ArchetypeToExemplar(const std::string& zArchName)
	{
		cExemplar out;
		auto itArch = mArchetypes.find(zArchName);
		if(itArch != mArchetypes.end())
		{
			const auto& arch = itArch->second;
			// Add tags
			out.mTags = arch.mTags;
			// Add components
			for (size_t i = 0;i < arch.mMask.size();++i)
				if(arch.mMask.test(i))
				{
					auto compo = CreateComponent( i);
					out.mComponents.AddComponent(compo);
				} 
		}
		return out;
	}

	//-------------------------------------------------------------------------------------------------
	cEntity cEntityMgr::InstantiateExemplar(const std::string& zExemplarName)
	{
		cEntityData ed;
		auto e = Create();
		auto itEx = mExemplars.find(zExemplarName);
		ed.mName = zExemplarName;// +"_instance";
		ed.mComponents = itEx->second.mComponents;

		auto it = mEntityData.find(e);
		it->second = ed;
		evt::cComponentsAdded::mSig.emit(it);

		// Add tags
		for (const auto& t : itEx->second.mTags)
			Tag(e,t);

		return e;
	}

	//-------------------------------------------------------------------------------------------------
	template<>
	void to_json<cEntityMgr>(const cEntityMgr& zMgr, JsonWriter& writer)
	{
		writer.StartObject();

		// Archetypes
		JsonWriter_AddMember("Archetypes", zMgr.mArchetypes, writer);

		// Exemplars
		JsonWriter_AddMember("Exemplars", zMgr.mExemplars, writer);

		// Tagged entities
		JsonWriter_AddMember("Tagged Entities", zMgr.mTaggedEntities, writer);

		// Entity Components
		JsonWriter_AddMember("Entity data", zMgr.mEntityData, writer);

		// Component types:
		JsonWriter_AddMember("ComponentTypeIds", zMgr.mComponentTypeIds, writer);

		writer.EndObject();
	}
}