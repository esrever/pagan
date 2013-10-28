#pragma once

#include <map>
#include <string>
#include <vector>

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
}