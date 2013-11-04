#pragma once

#include "AsciiWindow.h"

#include <core/util/jsonutils.h>

namespace pgn
{
	cAsciiWindow::cAsciiWindow()
	:mStart(0,0)
	,mSize(0, 0)
	{}

	void cAsciiWindow::Destroy()
	{
		if(mWindow.use_count() == 1)
		{
			wborder(*mWindow.get(), ' ', ' ', ' ',' ',' ',' ',' ',' ');
			wrefresh(*mWindow.get());
			delwin(*mWindow.get());
		}
	}
	
	void cAsciiWindow::Init()
	{
		Destroy();
		if (mSize.x && mSize.y)
		{
			mWindow = std::make_shared<WINDOW *>(newwin(mSize.y, mSize.x, mStart.y, mStart.x));
			box(*mWindow.get(), 0, 0);		/* 0, 0 gives default characters
						 * for the vertical and horizontal
						 * lines			*/
			//wrefresh(mWindow.get());		/* Show that box 		*/
		}
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