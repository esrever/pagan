#pragma once

#include "MoveNWAction.h"

namespace pgn
{
	namespace action
	{
		void MoveNW(cEntity e) {MoveAdjacent(e,-1,1);}
	}
}