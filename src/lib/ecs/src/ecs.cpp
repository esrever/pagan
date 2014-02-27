#include "ecs.h"
#include "EntityData.h"

#include <pystring.h>

namespace pgn
{
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
		mComponentTypeNamesToIds[result.back()] = mComponentTypeIds.size() - 1;
		return mComponentTypeIds.size() - 1;
	}

	//---------------------------------------------------------------------------------------------------
	void cECS::Tag(const std::string& tag, cEntityWithData ed)
	{
		mTagsToEntities[tag].insert(ed->first);
		ed->second.mTags.push_back(tag);
	}

	//---------------------------------------------------------------------------------------------------
	void cECS::Untag(const std::string& tag, cEntityWithData ed)
	{
		mTagsToEntities[tag].erase(ed->first);
		ed->second.mTags.remove(tag);
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const cECS& value) 
	{ 
		// export a dump of the object
		SerializeOut(writer, "EntitiesToData", value.EntitiesToData());
		SerializeOut(writer, "Archetypes", value.Archetypes());
		SerializeOut(writer, "TagsToEntities", value.TagsToEntities());
		SerializeOut(writer, "Queries", value.Queries());
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, cECS& value)
	{
		// Read in entities, etc
		return false;
	}
}