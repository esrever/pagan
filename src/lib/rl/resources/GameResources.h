#pragma once

#include <core/util/json_conversions.h>

#include "TileLib.h"
#include "FontLib.h"

namespace pgn
{
	struct cGameResources
	{
		cTileLib mTileLib;
		cFontLib mFontLib;
	};

	DECL_JSON_PGN(cGameResources)
}