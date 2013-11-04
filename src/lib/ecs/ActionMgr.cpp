#pragma once

#include "ActionMgr.h"

#include <map>
#include <string>


namespace pgn
{
	//----------------------------------------------------
	void cActionMgr::AddAction(const std::string& zName, action_fun zFun)
	{
		auto it = mActionNamesToIds.find(zName);
		if( it == mActionNamesToIds.end())
		{
			mActionNamesToIds[zName] = mActions.size();
			mActions.push_back(zFun);
		}
	}
}