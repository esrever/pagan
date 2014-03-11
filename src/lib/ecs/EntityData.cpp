#include "EntityData.h"
#include "componentbase.h"
#include "ecs.h"

//#include <ecs/serialize_ecs.h>

namespace pgn
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
		for (size_t i = 0; i < arch.mComponents.size();++i)
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
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const std::vector<cComponentBase_sptr> & value)
	{
		auto& cchild = writer.append_child(key.c_str());
		for (const auto& c : value)
		{
			if (c)
			{
				SerializeOut(cchild, ECS().ComponentTypeNames[c->TypeIndex()], c);
			}
		}
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, const std::string& key, std::vector<cComponentBase_sptr> & value)
	{
		for (auto it = reader.begin(); it != reader.end(); ++it)
		{

		}
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const cEntityData & value)
	{
		auto& child = writer.append_child(key.c_str());
		SerializeOut(child, "Name", value.mName);
		auto& cchild = child.append_child("Components");
		for (const auto& c : value.mComponents)
		{
			if (c)
			{
				SerializeOut(cchild, "array_elem", c);
			}
		}
		SerializeOut(child, "SupportMask", value.mSupportMask);
		SerializeOut(child, "SharedMask", value.mSharedMask);
		SerializeOut(child, "Archetype", value.mArchetype ? value.mArchetype->mName : "none");
		SerializeOut(child, "Tags", value.mTags);
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, cEntityData & value)
	{
		// Read name
		if( !SerializeIn(reader, "Name", value.mName)) return false;

		// Read archetype name
		std::string archname;
		SerializeIn(reader, "Archetype", archname);
		if (archname != "")
		{ 
			// get archetype
			auto it_arch = ECS().Archetypes().find(archname);
			if (it_arch == ECS().Archetypes().end()) return false; // Archetype not found
			auto& arch = it_arch->second;

			value.SetArchetype(arch);
		}

		// Read components
		auto& child = reader.child("Components");
		for (auto it = child.begin(); it != child.end(); ++it)
		{
			// get current component
			auto& cmpnode = *it;
			// get component type name
			std::string cmpname; 
			SerializeIn(cmpnode, cmpname);
			// get component type index
			auto it_cmpindex = ECS().ComponentTypeNamesToIds().find(cmpname);
			// sanity check
			if (it_cmpindex == ECS().ComponentTypeNamesToIds().end()) return false;
			// create typed component pointer
			auto cmp_ptr = ECS().ComponentCreators().at(it_cmpindex->second)();
			// Serialize in the component data
			SerializeIn(cmpnode, *cmp_ptr); 
			// Add it to the list!
			value.AddComponent(cmp_ptr);
		}

		// Read tags
		SerializeIn(reader, "Tags", value.mTags);
		return true;
	}
}