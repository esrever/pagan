#include "EntityData.h"
#include "componentbase.h"

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
	void SerializeOut(node_type& writer, const std::string& key, const cEntityData & value)
	{
		auto& child = writer.append_child(key.c_str());
		SerializeOut(child, "Components", value.mComponents);
		SerializeOut(child, "SupportMask", value.mSupportMask);
		SerializeOut(child, "SharedMask", value.mSharedMask);
		SerializeOut(child, "Archetype", value.mArchetype);
		SerializeOut(child, "Tags", value.mTags);
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, cEntityData & value)
	{
		return false;
	}
}