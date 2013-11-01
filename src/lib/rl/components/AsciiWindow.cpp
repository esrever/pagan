#pragma once

#include "AsciiWindow.h"

#include <core/util/jsonutils.h>

namespace pgn
{
	void cAsciiWindow::Destroy()
	{
		if(mWindow)
		{
			wborder(mWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
			wrefresh(mWindow);
			delwin(mWindow);
			mWindow = nullptr;
		}
	}
	
	void cAsciiWindow::Init()
	{
		Destroy();	
		mWindow = newwin(mSize.y, mSize.x, mStart.y, mStart.x);
		box(mWindow, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
		wrefresh(mWindow);		/* Show that box 		*/
	}

	template<>
	bool from_json<cAsciiWindow>(cAsciiWindow& zObj, const rapidjson::Value& zRoot)
	{
		from_json(zObj.mStart,"Start");
		from_json(zObj.mSize,"Size");
		zObj.Init();
		return true;
	}
}