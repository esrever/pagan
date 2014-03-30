#include "ecs.h"
#include "EntityData.h"
#include "events.h"

#include <pystring.h>

namespace pgn
{
	namespace ecs
	{
		cECS& ECS()
		{
			return *mainecs();
		}

		//---------------------------------------------------------------------------------------------------
		cEntityWithData cECS::Create(const cEntityData& ed)
		{
			const auto& e = mEntityIdGen.New();
			auto it = mEntitiesToData.emplace(e, ed);
			for (const auto& tag : ed.mTags)
				mTagsToEntities[tag].insert(it.first);
			for (const auto& tag : ed.mTagus)
				mTagusToEntities[tag] = it.first;
			evt::cEntityCreated::Sig().emit(it.first);
			return it.first;
		}

		//---------------------------------------------------------------------------------------------------
		void cECS::Destroy(cEntityWithData ed)
		{
			evt::cEntityDestroy::Sig().emit(ed);

			// erase from unique tags
			auto itu = std::find_if(mTagusToEntities.begin(), mTagusToEntities.end(), [&](const std::pair<std::string, cEntityWithData>& e){return e.second->first == ed->first; });
			if (itu != mTagusToEntities.end())
				mTagusToEntities.erase(itu);

			// erase from tags
			auto it = mTagsToEntities.begin();
			while (it != mTagsToEntities.end())
			{
				auto itc = it++;
				auto& kv = *itc;
				kv.second.erase(ed);
				if (kv.second.empty())
					mTagsToEntities.erase(itc);
			}
			mEntitiesToData.erase(ed->first);
		}

		//---------------------------------------------------------------------------------------------------
		unsigned short cECS::AddComponentType(const std::type_index& zTi)
		{
			// if it's already in our list, do nothing and return the index
			for (size_t i = 0; i < mComponentTypeIds.size(); ++i)
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
			pystring::rsplit(tmp, result, "::c", 1);
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
			auto it = mTagsToEntities.find(tag);
			if (it != mTagsToEntities.end())
			{
				it->second.erase(ed);
				ed->second.mTags.erase(tag);
			}
		}

		//---------------------------------------------------------------------------------------------------
		void cECS::Tagu(const std::string& tag, cEntityWithData ed)
		{
			mTagusToEntities[tag] = ed;
			ed->second.mTagus.insert(tag);
		}

		//---------------------------------------------------------------------------------------------------
		void cECS::Untagu(const std::string& tag, cEntityWithData ed)
		{
			mTagusToEntities.erase(tag);
			ed->second.mTagus.erase(tag);
		}

		//---------------------------------------------------------------------------------------------------
		cEntityWithData cECS::InstantiateArchetype(const cEntityData& arch)
		{
			// generate entity
			cEntityWithData ewd = Create();

			// copy the data
			cEntityData& ed = ewd->second;
			ed.SetArchetype(arch);

			// for each component that is in the support but not in the shared mask, instantiate
			for (size_t i = 0; i < ed.mComponents.size();++i)
			{
				auto& cmp = ed.mComponents[i];
				if (arch.mComponents[i] && (!ed.mShareMask.at(i)))
				{
					ed.AddComponent(mComponentCreators[i]());
					ed.mComponents[i]->from(*arch.mComponents[i]);
					evt::cComponentAdded::Sig().emit(ewd, i);
				}
			}
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
					bool is_archetype = false;
					pgn::SerializeIn(ent, "is_archetype", is_archetype);
					if (!is_archetype)
						Create(ed);
					else
						mArchetypes.emplace(ed.mName, ed);
				}
			}
		}

		//---------------------------------------------------------------------------------------------------
		void cECS::RegisterAction(const std::string& s, cActionFunc func)
		{
			mActionFuncs[s] = func;
		}
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const ecs::cECS& value)
	{
		// export a dump of the object
		SerializeOut(node.append_child("EntitiesToData"), value.EntitiesToData());
		SerializeOut(node.append_child("Archetypes"), value.Archetypes());
		SerializeOut(node.append_child("TagsToEntities"), value.TagsToEntities());
		SerializeOut(node.append_child("Queries"), value.Queries());
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, ecs::cECS& value)
	{
		value.ParseEntities(reader.child("Entities"));
		return 1;
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const ecs::cActionBindings& value)
	{
		
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, ecs::cActionBindings& value)
	{
		std::map<std::string, std::string> bindings;
		size_t ret = SerializeIn(reader, bindings);
		for (const auto& kv : bindings)
			value[std::from_string<int>(kv.first)] = mainecs()->ActionFuncs(kv.second)->second;
		return 1;
	}
}