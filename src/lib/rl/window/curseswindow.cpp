#pragma once

#include "curseswindow.h"

#include <core/util/json_conversions_glm.h>
#include <core/util/jsonutils.h>

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
	
	void cCursesWindow::Init(const glm::ivec2& zStart, const glm::uvec2& zSize, WINDOW * parent)
	{
		mStart = zStart;
		mSize = zSize;
		Init(parent);
	}

	void cCursesWindow::Init(WINDOW * parent)
	{
		Destroy();	
		if(!parent)
			mWindow = newwin(mSize.y, mSize.x, mStart.y, mStart.x);
		else
			mWindow = derwin(parent, mSize.y, mSize.x, mStart.y, mStart.x);
		box(mWindow, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
		wrefresh(mWindow);		/* Show that box 		*/
	}

	template<>
	bool from_json<cCursesWindow>(cCursesWindow& zObj, const rapidjson::Value& zRoot)
	{
		from_json(zObj.mName,"name");
		from_json(zObj.mStart,"pos");
		from_json(zObj.mSize,"size");
		read_json_vector(zObj.mSubWindows,"subwindows");
		return true;
	}
}