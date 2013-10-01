#include "conversions.h"

#include <pystring.h>

namespace pgn
{
	//-------------------------------------------------------------------------------------
	std::string pad_string(const std::string& zText, const std::string& zPad)
	{
		auto n = pystring::count(zText,"\n");
		return zPad + pystring::replace(zText,"\n","\n"+zPad,n-1);
	}

	//-------------------------------------------------------------------------------------
	std::string repeat_string(const std::string& s, int count) 
	{ 
		std::string ret;
		for(auto i=0; i<count; ++i)
			ret += s;
		return ret;
	}
}