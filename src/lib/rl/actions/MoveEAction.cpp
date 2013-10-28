#pragma once

#include "MoveEAction.h"

namespace pgn
{
	namespace action
	{
		void MoveE(cEntity e) {MoveAdjacent(e,1,0);}
	}
}