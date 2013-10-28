#pragma once

#include "MoveSAction.h"

namespace pgn
{
	namespace action
	{
		void MoveS(cEntity e) {MoveAdjacent(e,0,-1);}
	}
}