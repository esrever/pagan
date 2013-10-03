#include "conversions.h"

#include <fstream>

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

	//----------------------------------------------------------------------------------------
	std::string file_to_text(const std::string& fname)
	{
		std::ifstream in(fname, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize( size_t(in.tellg()));
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}
}