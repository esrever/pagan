#include "AsciiRenderSystem.h"

#include <curses.h>

namespace pgn
{
	void cAsciiRenderSystem::Process()
	{
		// TODO: For each entity in the level query, get asciilevelrep & render
		// TODO: For each entity in the character query, get asciirep & render
		refresh();
	}

	void cAsciiRenderSystem::from_json(const rapidjson::Value& zRoot)
	{
		assert(false);
	}
}