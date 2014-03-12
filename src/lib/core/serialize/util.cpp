#include "util.h"

#include <ostream>
#include <fstream>
#include <vector>

#include <pugixml.hpp>

namespace pgn
{
	bool LoadXML(pugi::xml_document& doc, const char * fname, std::ostream& os)
	{
		auto res = doc.load_file(fname);
		if (!res)
		{
			std::ifstream ifs(fname);
			std::vector<char> chars(res.offset + 1);
			ifs.read(&chars.front(), res.offset);
			chars.back() = '\0';
			os << "//------------------------------------------------------------------\n";
			os << res.offset << ": " << res.description() << std::endl;
			os << &chars.front() << std::endl;
			os << "//------------------------------------------------------------------\n";
		}
		return res;
	}
}