#pragma once

#include <map>
#include <string>

namespace pgn
{
	typedef std::map<std::string, std::string> cDict;

	//-----------------------------------------------------------------------
	void EvalFormatString(const cDict& zFs, std::string& sOut);
}
