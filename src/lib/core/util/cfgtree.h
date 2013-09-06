#pragma once

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <pystring.h>

#include "conversions.h"
#include "cfgdata.h"

namespace pgn
{
	/* 
		DESCRIPTION: Configuration tree creator

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
			void BuildTree(const rapidjson::Value& val, std::vector<std::string> zRunningName);

		public:
			cCfgData mCfgData;
	};

	//-----------------------------------------------------------------------
	template <>
	inline std::string to_string<cCfgTree>(const cCfgTree& s)
	{
		std::string tot = "";
		for(auto v : s.mCfgData.Data())
		{
			tot += v.first + " : " + pystring::join(", ", v.second) + "\n";
		}
		return tot;
	}


	
}