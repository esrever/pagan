#pragma once

#include "cfgdata.h"

namespace pgn
{
	/* 
		DESCRIPTION: Configuration tree

		From nothing (empty)
		From json file
		from opts (be aware of jopt and unpack/merge that)
	*/

	class cCfgTree
	{
		public:
			cCfgTree(){}
			cCfgTree(const std::string& zJsonFileName);
			cCfgTree(const cCfgData& zCfgData);
		
		private:
			void BuildTreeLevel(const rapidjson::Value& val, int depth, std::string& zRunningName);

		public:
			cCfgData mCfgData;
	};


	
}