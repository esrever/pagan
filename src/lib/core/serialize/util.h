#pragma once

#include <iosfwd>
#include <string>

namespace pugi
{
	class xml_document;
	class xml_node;
}

namespace pgn
{
	bool LoadXML(pugi::xml_document& doc, const char * fname, std::ostream& os);
	std::string DisplayNodeAndChildren(const pugi::xml_node& node);
}