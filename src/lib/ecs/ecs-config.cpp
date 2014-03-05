#include "ecs-config.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const std::string& key, const cComponentMask& value) 
	{ 
		SerializeOut(writer, key, value.to_string()); 
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
		return false;
	}
}