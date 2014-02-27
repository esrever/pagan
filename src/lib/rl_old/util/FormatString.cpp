#include "FormatString.h"

#include <pystring.h>

namespace pgn
{
	//-----------------------------------------------------------------------
	void EvalFormatString(const cDict& zFs, std::string& sOut)
	{
		static const std::string prefix = "${";
		static const std::string suffix = "}";
		for (auto x : zFs)
			sOut = pystring::replace(sOut, prefix + x.first + suffix, x.second);
	}
}
