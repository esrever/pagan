#include "ecs-config.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const cComponentMask& value)
	{
		// export a dump of the object
		SerializeOut(writer, value.to_string());
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, cComponentMask& value)
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