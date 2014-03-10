#include "tileinfo.h"

#include <rlut/app/rlapp.h>

namespace pgn
{
	namespace rl
	{
		cTileInfo::cTileInfo()
		{
			mBg = mFg = mActor = mainecs()->EntitiesToData().end();
		}
	}
}