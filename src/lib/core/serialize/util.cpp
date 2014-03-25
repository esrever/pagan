#include "util.h"

#include <ostream>
#include <fstream>
#include <vector>

#include <pugixml.hpp>

#include <core/util/string.h>

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

	std::string DisplayNodeAndChildren(const pugi::xml_node& node)
	{
		std::string str_children;
		for (auto c : node.children())
		{
			str_children += format("%s ,", c.name());
		}
		return node.name() + std::string(": ") + str_children;
	}
}