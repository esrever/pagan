#include "EntityData.h"
#include "componentbase.h"
#include "ecs.h"

#include <core/app/sdlapp.h>

namespace pgn
{
	namespace ecs
	{
		//---------------------------------------------------------------------------------------------------
		void cEntityData::AddComponent(cComponentBase_sptr comp)
		{
			auto idx = comp->TypeIndex();
			mSupportMask.set(idx);
			mSharedMask.reset(idx);
			if (mComponents.size() <= idx)
				mComponents.resize(idx + 1);
			mComponents[idx] = comp;
		}

		//---------------------------------------------------------------------------------------------------
		void cEntityData::SetArchetype(const cEntityData& arch)
		{
			// set the archetype
			mArchetype = &arch;
			// archetype's support is the instance's shared
			mSupportMask |= arch.mSharedMask;
			mSharedMask = arch.mSupportMask;

			// allocate enough components to accomodate archetype's components
			if (mComponents.size() < arch.mComponents.size())
				mComponents.resize(arch.mComponents.size());

			// Set the shared components
			for (size_t i = 0; i < arch.mComponents.size(); ++i)
			{
				if (arch.mSupportMask.at(i) && arch.mComponents.at(i))
				{
					mComponents.at(i) = arch.mComponents.at(i);
					mSharedMask.at(i) = 1;
				}
				else
					mSharedMask.at(i) = 0;
			}

			// add tags
			mTags.insert(arch.mTags.begin(), arch.mTags.end());
			mTagus.insert(arch.mTagus.begin(), arch.mTagus.end());
		}


	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const std::vector<ecs::cComponentBase_sptr> & value)
	{
		for (const auto& c : value)
		{
			if (c)
				SerializeOut(node.append_child(mainecs()->ComponentTypeNames()[c->TypeIndex()].c_str()), c);
		}
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, std::vector<ecs::cComponentBase_sptr> & value)
	{
		size_t ret = 0;
		for (auto it = reader.begin(); it != reader.end(); ++it)
		{
			auto itid = mainecs()->ComponentTypeNamesToIds().find(it->name());
			if (itid != mainecs()->ComponentTypeNamesToIds().end())
			{
				auto ptr = mainecs()->ComponentCreators().at(itid->second)();
				//ret += ptr.get()->SerializeIn(*it);
				ret += SerializeIn(*it, *ptr);
				value.push_back(ptr);
			}
			else
				return 0;
		}
		return ret;
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const ecs::cEntityData & value)
	{
		SerializeOut(node, "name", value.mName);
		if (value.mArchetype)
			SerializeOut(node, "archetype", value.mArchetype->mName);
		SerializeOut(node, "Components", value.mComponents);
		SerializeOut(node, "SupportMask", value.mSupportMask);
		SerializeOut(node, "SharedMask", value.mSharedMask);
		SerializeOut(node, "Tags", value.mTags);
		SerializeOut(node, "Tagus", value.mTagus);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, ecs::cEntityData & value)
	{
		// Read name
		if (SerializeIn(reader, "name", value.mName) == 0) return false;

		// Read archetype name
		std::string archname;
		if (SerializeIn(reader, "archetype", archname))
		{
			// get archetype
			auto it_arch = mainecs()->Archetypes().find(archname);
			if (it_arch == mainecs()->Archetypes().end()) return 0; // Archetype not found
			auto& arch = it_arch->second;

			value.SetArchetype(arch);
		}

		// Read components
		std::vector<ecs::cComponentBase_sptr> cmps;
		SerializeIn(reader, "Components", cmps);
		for (auto& cmp : cmps)
		{
			// Add it to the list!
			value.AddComponent(cmp);
		}

		// Read tags
		SerializeIn(reader, "Tags", value.mTags);
		SerializeIn(reader, "Tagus", value.mTagus);
		return true;
	}
}