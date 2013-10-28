#pragma once

#include <map>
#include <string>
#include <vector>

#include "Event.h"


namespace pgn
{
	typedef Gallant::Delegate1<cEntity> action_fun;

	class cActionMgr
	{		
		public:
			virtual ~cActionMgr(){}

			virtual void RegisterAllActions(){}
			void AddAction(const std::string& zName, action_fun zFun);

			action_fun GetAction(size_t zIdx) {return mActions.at(zIdx);}
			action_fun GetAction(const std::string& zName) {return GetAction( mActionNamesToIds[zName]);}
		private:
			std::map< std::string, size_t > mActionNamesToIds;
			std::vector<action_fun> mActions;
	};
}