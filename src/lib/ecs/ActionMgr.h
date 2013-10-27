#pragma once

#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "Event.h"


namespace pgn
{
	class cActionMgr
	{
		public:
			typedef Gallant::Delegate1<cEntity> action_fun;
		public:
			virtual ~cActionMgr(){}

			virtual void RegisterAllActions(){}
			void AddAction(const std::string& zName, action_fun zFun);
		private:
			std::map< std::string, size_t > mActionNamesToIds;
			std::vector<action_fun> mActions;
	};

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