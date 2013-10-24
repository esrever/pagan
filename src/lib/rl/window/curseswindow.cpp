#pragma once

#include "curseswindow.h"

namespace pgn
{
	void cCursesWindow::Destroy()
	{
		if(mWin)
		{
			wborder(mWin, ' ', ' ', ' ',' ',' ',' ',' ',' ');
			wrefresh(mWin);
			delwin(mWin);
			mWin = nullptr;
		}
	}
	void cCursesWindow::Init(const glm::ivec2& zStart, const glm::uvec2& zSize)
	{
		Destroy();	
		mWin = newwin(h, w, y, x);
		box(mWin, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
		wrefresh(mWin);		/* Show that box 		*/
	}
}