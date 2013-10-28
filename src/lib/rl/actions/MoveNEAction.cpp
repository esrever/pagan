#pragma once

#include "MoveNEAction.h"

namespace pgn
{
	namespace action
	{
		void MoveNE(cEntity e) {MoveAdjacent(e,1,1);}
	}
}