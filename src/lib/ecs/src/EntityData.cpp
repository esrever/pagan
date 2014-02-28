#include "EntityData.h"
#include "componentbase.h"
#include "ecs.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void cEntityData::AddComponent(cComponentBase_sptr comp)
	{
		auto idx = comp->TypeIndex();
		assert(mSupportMask.at(idx));
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
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const cEntityData & value)
	{
		auto& child = writer.append_child(key.c_str());
		SerializeOut(child, "Name", value.mName);
		SerializeOut(child, "Components", value.mComponents);
		SerializeOut(child, "SupportMask", value.mSupportMask);
		SerializeOut(child, "SharedMask", value.mSharedMask);
		SerializeOut(child, "Archetype", value.mArchetype);
		SerializeOut(child, "Tags", value.mTags);
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, cEntityData & value)
	{
		/*
			SerializeIn uses a particular format:
				Name : string
				Archetype : string
				Define: vector<object> : 
					<elem value="component_type">
						<param0/>
						<param1/>
						...
				Tags: vector<string>
		*/

		// Read name
		if( !SerializeIn(reader, "Name", value.mName)) return false;

		// Read archetype name
		std::string archname;
		SerializeIn(reader, "Name", archname);
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
			// TODO: add some checks here. Do I really need to access the XML reader's interface?
			// generate typed component
			auto it_cmpindex = ECS().ComponentTypeNamesToIds().find(cmpnode.attribute("value").as_string());
			auto cmp_ptr = ECS().ComponentCreators().at(it_cmpindex->second)();
			// Serialize in the component data
			SerializeIn(cmpnode, *cmp_ptr); // TODO: this is dodgy. I expect it to call .SerializeIn() from it's derived function: cComponent<T>. 
			// Add it to the list!
			value.AddComponent(cmp_ptr);
		}

		// Read tags
		SerializeIn(reader, "Tags", value.mTags);
		return true;
	}
}