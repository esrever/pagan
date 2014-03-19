#include "ecs-config.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& writer, const ecs::cComponentMask& value)
	{
		// export a dump of the object
		SerializeOut(writer, value.to_string());
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& reader, ecs::cComponentMask& value)
	{
		std::string s;
		if (SerializeIn(reader, s))
		{
			value = ecs::cComponentMask(s);
			return true;
		}
		else 
			return false;
	}
}