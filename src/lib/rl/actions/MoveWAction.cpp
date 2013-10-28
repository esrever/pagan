#pragma once

#include "MoveWAction.h"
#include "common/MoveAdjacent.h"

namespace pgn
{
	namespace action
	{
		void MoveW(cEntity e) {MoveAdjacent(e,-1,0);}
	}
}