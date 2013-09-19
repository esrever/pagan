#include "tile.h"

#include <cassert>

template<>
void read_json<cTile>(cTile& obj, const rapidjson::Document& doc)
{
	// TODO: cast this to the base class and run it, then here and run it again
	assert(false);
}