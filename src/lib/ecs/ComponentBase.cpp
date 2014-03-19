#include "ComponentBase.h"

namespace pgn
{
	namespace ecs
	{
	}

	void SerializeOut(node_type& writer, const ecs::cComponentBase & value){ value.SerializeOut(writer); }
	size_t SerializeIn(const node_type& reader, ecs::cComponentBase & value){ return value.SerializeIn(reader); }
}