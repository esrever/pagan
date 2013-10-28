#pragma once

#include "MoveNAction.h"

namespace pgn
{
	namespace action
	{
		void MoveN(cEntity e) {MoveAdjacent(e,0,1);}
	}
}