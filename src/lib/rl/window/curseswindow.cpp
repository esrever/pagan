#pragma once

#include "curseswindow.h"

namespace pgn
{
	void cCursesWindow::Destroy()
	{
		if(mWindow)
		{
			wborder(mWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
			wrefresh(mWindow);
			delwin(mWindow);
			mWindow = nullptr;
		}
	}
	void cCursesWindow::Init(const glm::ivec2& zStart, const glm::uvec2& zSize)
	{
		Destroy();	
		mWindow = newwin(zSize.y, zSize.x, zStart.y, zStart.x);
		box(mWindow, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
		wrefresh(mWindow);		/* Show that box 		*/
	}
}