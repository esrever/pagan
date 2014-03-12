#include "level.h"

#include <rlut/dungen/dungen.h>

namespace pgn
{
	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const rl::cLevel& value)
	{
		
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, const rl::cLevel& value)
	{
		size_t ret = 0;
		pgn::rlut::cWorkspace ws;
		pgn::SerializeIn(node, "DunGen", ws);
		std::map<std::string, std::string> tiles;
		pgn::SerializeIn(node, "Tiles", tiles);
		
		// TODO: fetch the entities in "tiles"
		// set the appropriate bg/fg tiles

		auto& ecs = mainecs();
		return ret;
	}
}