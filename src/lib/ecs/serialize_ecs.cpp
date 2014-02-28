#include "ecs-config.h"

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	template<> void SerializeOut<cComponentMask>(node_type& writer, const std::string& key, const cComponentMask& value)
	{
		// export a dump of the object
		SerializeOut(writer, key.c_str(), value.to_string());
	}

	//---------------------------------------------------------------------------------------------------
	template<> bool SerializeIn<cComponentMask>(const node_type& reader, cComponentMask& value)
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