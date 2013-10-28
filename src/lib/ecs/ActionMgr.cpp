#pragma once

#include "ActionMgr.h"

#include <map>
#include <string>


namespace pgn
{
	//----------------------------------------------------
	void cActionMgr::AddAction(const std::string& zName, action_fun zFun)
	{
		auto it = std::find(mActions.begin(), mActions.end(),zFun);
		size_t idx = mActions.size();
		if( it == mActions.end())
		{
			mActionNamesToIds[zName] = mActions.size();
			mActions.push_back(zFun);
		}
	}
}