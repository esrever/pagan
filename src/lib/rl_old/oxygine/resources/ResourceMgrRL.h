#pragma once

#include <core/util/singleton.h>

#include "TileLib.h"
#include "FontLib.h"

#define ResourceInstance pgn::cSingleton<pgn::cResourceMgrRL>::Instance()

namespace pgn
{
	struct cResourceMgrRL
	{
		cResourceMgrRL() { mTileLib.Init("",""); mFontLib.Init(""); }

		cTileLib mTileLib;
		cFontLib mFontLib;
	};
}