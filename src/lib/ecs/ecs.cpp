#include "ecs.h"
#include "EntityData.h"

#include <pystring.h>

namespace pgn
{
	cECS& ECS()
	{
		return *mainecs();
	}

	//---------------------------------------------------------------------------------------------------
	cECS::cEntityWithData cECS::Create(const cEntityData& ed)
	{
		const auto& e = mEntityIdGen.New();
		auto it = mEntitiesToData.emplace(e, ed);
		for (const auto& tag : ed.mTags)
			mTagsToEntities[tag].insert(it.first);
		return it.first;
	}

	//---------------------------------------------------------------------------------------------------
	unsigned short cECS::AddComponentType(const std::type_index& zTi)
	{
		// if it's already in our list, do nothing and return the index
		for (size_t i = 0; i<mComponentTypeIds.size(); ++i)
		{
			if (zTi == mComponentTypeIds[i])
				return i;
		}
		// add the type & the creator
		mComponentTypeIds.push_back(zTi);
		mComponentCreators.push_back(ComponentCreatorFunction());

		// create a nice name & add it, omit the struct from "struct blah"
		std::vector<std::string> result;
		pystring::split(zTi.name(), result, " ");
		std::string tmp = result.back();
		pystring::rsplit(tmp, result, "::c",1);
		tmp = pystring::strip(result.back(), ">");
		auto idx = mComponentTypeIds.size() - 1;
		mComponentTypeNamesToIds[tmp] = idx;
		mComponentTypeNames.push_back(tmp);
		return mComponentTypeIds.size() - 1;
	}

	//---------------------------------------------------------------------------------------------------
	void cECS::Tag(const std::string& tag, cEntityWithData ed)
	{
		mTagsToEntities[tag].insert(ed);
		ed->second.mTags.insert(tag);
	}

	//---------------------------------------------------------------------------------------------------
	void cECS::Untag(const std::string& tag, cEntityWithData ed)
	{
		mTagsToEntities[tag].erase(ed);
		ed->second.mTags.erase(tag);
	}

	//---------------------------------------------------------------------------------------------------
	cECS::cEntityWithData cECS::InstantiateArchetype(const cEntityData& arch)
	{
		// generate entity
		cEntityWithData ewd = Create();
		
		// copy the data
		cEntityData& ed = ewd->second;
		ed.SetArchetype(arch);
		return ewd;
	}

	//---------------------------------------------------------------------------------------------------
	void cECS::ParseEntities(const node_type& reader)
	{
		// We expect an entity list here
		for (auto it = reader.begin(); it != reader.end(); ++it)
		{
			const auto& ent = *it;

			// parse the entity data
			cEntityData ed;
			if (pgn::SerializeIn(ent, ed))
			{
				// determine if it's an archetype or not
				bool is_archetype=false;
				pgn::SerializeIn(ent, "is_archetype", is_archetype);
				if (!is_archetype)
					Create(ed);
				else
					mArchetypes.emplace(ed.mName,ed);
			}
		}
	}

	//---------------------------------------------------------------------------------------------------
	void cECS::RegisterAction(const std::string& s, cActionFunc func)
	{
		mActionFuncs[s] = func;
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const cECS& value) 
	{ 
		// export a dump of the object
		SerializeOut(node.append_child("EntitiesToData"), value.EntitiesToData());
		SerializeOut(node.append_child("Archetypes"), value.Archetypes());
		SerializeOut(node.append_child("TagsToEntities"), value.TagsToEntities());
		SerializeOut(node.append_child("Queries"), value.Queries());
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, cECS& value)
	{
		value.ParseEntities(reader.child("Entities"));
		return 1;
	}
}