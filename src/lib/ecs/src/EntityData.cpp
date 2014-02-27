#include "EntityData.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const cEntityData & value)
	{
		SerializeOut(writer, "Components", value.mComponents);
		SerializeOut(writer, "SupportMask", value.mSupportMask);
		SerializeOut(writer, "SharedMask", value.mSharedMask);
		SerializeOut(writer, "Archetype", value.mArchetype);
		SerializeOut(writer, "Tags", value.mTags);
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, cEntityData & value)
	{
		return false;
	}
}