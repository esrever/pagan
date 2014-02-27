#include "LogCurses.h"

#include <rl/components/AsciiWindow.h>

namespace pgn
{
	//--------------------------------------------------------------------------
	void cLogCurses::Log(const eLogLevel zLevel, const std::string& zMsg)
	{
		wprintw(*(mWindow->mWindow.get()),"%s\n",zMsg.c_str());
	}
}