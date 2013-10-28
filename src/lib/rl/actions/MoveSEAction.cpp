#pragma once

#include "MoveSEAction.h"

namespace pgn
{
	namespace action
	{
		void MoveSE(cEntity e) {MoveAdjacent(e,1,-1);}
	}
}