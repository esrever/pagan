#include "ComponentBase.h"

namespace pgn
{
	void SerializeOut(node_type& writer, const std::string& key, const cComponentBase & value){ value.SerializeOut(writer, key); }
	bool SerializeIn(const node_type& reader, cComponentBase & value){ return value.SerializeIn(reader); }
}