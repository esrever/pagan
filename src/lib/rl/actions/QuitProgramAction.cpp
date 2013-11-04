#pragma once

#include "QuitProgramAction.h"
#include "../events/events.h"

namespace pgn
{
	namespace action
	{
		void QuitProgram(cEntity e) 
		{
			cExitApplicationEvent::mSig.emit(); 
		}
	}
}