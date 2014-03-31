#include "EntityData.h"
#include "componentbase.h"
#include "ecs.h"
#include "events.h"

#include <core/app/sdlapp.h>

namespace pgn
{
	namespace ecs
	{
		//---------------------------------------------------------------------------------------------------
		void cEntityData::AddComponent(cComponentBase_sptr comp)
		{
			auto idx = comp->TypeIndex();
			mShareMask.reset(idx);
			if (mComponents.size() <= idx)
				mComponents.resize(idx + 1);
			mComponents[idx] = comp;
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
		SerializeOut(node, "Components", value.mComponents);
		SerializeOut(node, "SharedMask", value.mShareMask);
		SerializeOut(node, "Tags", value.mTags);
		SerializeOut(node, "Tagus", value.mTagus);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, ecs::cEntityData & value)
	{
		// Read archetype name. NOTE: it needs to be done before reading further components!
		std::string archname;
		if (SerializeIn(reader, "archetype", archname))
		{
			// get archetype
			auto it_arch = mainecs()->Archetypes().find(archname);
			if (it_arch == mainecs()->Archetypes().end()) return 0; // Archetype not found
			auto& arch = it_arch->second;

			value = arch;
		}

		// Read name. If it doesn't exist, we use the archetype's name
		std::string name;
		if (SerializeIn(reader, "name", name)) 
			value.mName = name;

		// Read components
		for (const auto& cmpnode : reader.child("Components").children())
		{
			auto itid = mainecs()->ComponentTypeNamesToIds().find(cmpnode.name());
			if (itid != mainecs()->ComponentTypeNamesToIds().end())
			{
				// create component
				auto ptr = mainecs()->ComponentCreators().at(itid->second)();
				// read data
				SerializeIn(cmpnode, *ptr);
				// add it to entitydata
				value.AddComponent(ptr);
				// if shared, mark it as such
				bool share = false;
				SerializeIn(cmpnode, "share", share);
				if (share)
					value.mShareMask[itid->second] = true;
			}
			else
				mainapp()->SysLog().Err(format("Component %s does not exist!", cmpnode.name()));
		}

		// Read and add tags 
		std::set<std::string> tags, tagus;
		SerializeIn(reader, "Tags", tags);
		SerializeIn(reader, "Tagus", tagus);
		value.mTags.insert(tags.begin(), tags.end());
		value.mTagus.insert(tagus.begin(), tagus.end());
		return true;
	}
}