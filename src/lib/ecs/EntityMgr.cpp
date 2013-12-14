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
	cEntityWithData cEntityMgr::Create()
	{ 
		const auto& e = mEntityIdGen.New();
		auto it = mEntityData.insert(std::pair<cEntity,cEntityData>(e,cEntityData()));
		evt::cEntityCreated::mSig.emit(it.first);
		return it.first;
	}

	//----------------------------------------------------------------
	void cEntityMgr::Destroy(cEntityWithData zEntity)
	{
		evt::cEntityDestroy::mSig.emit(zEntity);
		// and finally erase it
		mEntityIdGen.Destroy(zEntity->first);
		mEntityData.erase(zEntity);
	}

	//----------------------------------------------------------------
	void cEntityMgr::OnEntityCreated(cEntityWithData e)
	{
	}
	
	//----------------------------------------------------------------
	void cEntityMgr::OnEntityDestroy( cEntityWithData e)
	{
		// untag from all groups
		Untag(e);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Tag(cEntityWithData zEntity, const std::string& zTag)
	{
		if (zTag == "Player")
			mGlobals.mPC = zEntity;
		else if (zTag == "World")
			mGlobals.mWorld = zEntity;
		mEntityData[zEntity->first].mTags.insert(zTag);
		evt::cEntityTagged::mSig.emit(zEntity, zTag);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntityWithData zEntity, const std::string& zTag)
	{
		evt::cEntityUntag::mSig.emit(zEntity, zTag);
		mEntityData[zEntity->first].mTags.erase(zTag);
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(const std::string& zTag)
	{
		// Look for the tag
		foreach(ited,mEntityData)
		{
			Untag(ited, zTag);
		}
	}

	//----------------------------------------------------------------
	void cEntityMgr::Untag(cEntityWithData zEntity)
	{
		foreach(ited, mEntityData)
			if (ited->first == zEntity->first) // TODO: check for normal equality
			{
				for (const auto& tag : ited->second.mTags)
					evt::cEntityUntag::mSig.emit(zEntity, tag);
				ited->second.mTags.clear();
			}
	}

	//----------------------------------------------------------------
	void cEntityMgr::AddComponentPtr(cEntityWithData zEntity, cComponentBaseSptr zComponent)
	{
		zEntity->second.mComponents.AddComponent(zComponent);
		evt::cComponentAdded::mSig.emit(zEntity, zComponent->TypeIndex());
	}

	//----------------------------------------------------------------
	void cEntityMgr::RemoveComponentPtr(cEntityWithData zEntity, cComponentBaseWptr zComponent)
	{
		evt::cComponentRemove::mSig.emit(zEntity, zComponent.lock()->TypeIndex());
		zEntity->second.mComponents.RemoveComponent(zComponent.lock()->TypeIndex());
		//! No components -> destroy entity
		if (zEntity->second.mComponents.Mask().none())
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
				archetype.mTags.insert( tags.begin(), tags.end());
				for( const auto& x : inherited)
				{
					auto iter = mArchetypes.find(x);
					if (iter != mArchetypes.end())
					{
						archetype.mMask |= iter->second.mMask;
						archetype.mTags.insert(iter->second.mTags.begin(), iter->second.mTags.end());
					}
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
					exemplar.mTags.insert( tags.begin(), tags.end());
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
	cEntityWithData cEntityMgr::InstantiateExemplar(const std::string& zExemplarName)
	{
		cEntityData ed;
		auto itEx = mExemplars.find(zExemplarName);
		if (itEx == mExemplars.end())
			return mEntityData.end();
		auto e = Create();
		ed.mName = zExemplarName;// +"_instance";
		ed.mComponents = itEx->second.mComponents;
		ed.mTags = itEx->second.mTags;

		e->second = ed;
		evt::cComponentsAdded::mSig.emit(e);

		// Add tags
		for (const auto& t : itEx->second.mTags)
			Tag(e,t);

		return e;
	}

	cEntityWithData cEntityMgr::CloneEntity(cEntityWithData ed)
	{
		// TODO: tags?
		auto e = Create();
		cEntityData edout;
		auto& edin = ed->second;
		edout.mName = edin.mName;
		edout.mTags = edin.mTags;
		edout.mComponents = edin.mComponents.Clone();
		e->second = edout;
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

		// Entity Components
		JsonWriter_AddMember("Entity data", zMgr.mEntityData, writer);

		// Component types:
		JsonWriter_AddMember("ComponentTypeIds", zMgr.mComponentTypeIds, writer);

		writer.EndObject();
	}

	void cEntityMgr::TaggedEntities(std::vector<cEntityWithData>& e, const std::string s)
	{
		e.clear();
		foreach(ited, mEntityData)
		{
			auto it = std::find(ited->second.mTags.begin(), ited->second.mTags.end(), s);
			if (it != ited->second.mTags.end())
				e.push_back(ited);
		}
	}

	cEntityWithData cEntityMgr::TaggedEntity(const std::string s)
	{
		std::vector<cEntityWithData> v;
		TaggedEntities(v, s);
		assert(v.size() == 1);
		return v.at(0);
	}
}