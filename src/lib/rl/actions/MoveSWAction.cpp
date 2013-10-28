#pragma once

#include "MoveSWAction.h"

namespace pgn
{
	namespace action
	{
		void MoveSW(cEntity e) {MoveAdjacent(e,-1,-1);}
	}
}