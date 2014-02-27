#include "EntityData.h"

namespace pgn
{
	void SerializeOut(node_type& writer, const std::string& key, const cEntityData & value)
	{
		SerializeOut(writer, "components", value.mComponents);
	}

	bool SerializeIn(const node_type& reader, cEntityData & value)
	{
		return true;
	}
}