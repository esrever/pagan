#include "serialize_ecs.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const cComponentMask& value)
	{
		// export a dump of the object
		SerializeOut(writer, key.c_str(), value.to_string());
	}

	//---------------------------------------------------------------------------------------------------
	bool SerializeIn(const node_type& reader, cComponentMask& value)
	{
		std::string s;
		if (SerializeIn(reader, s))
		{
			value = cComponentMask(s);
			return true;
		}
		else 
			return false;
	}
}