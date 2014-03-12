#pragma once

#include <iosfwd>

namespace pugi
{
	class xml_document;
}

namespace pgn
{
	bool LoadXML(pugi::xml_document& doc, const char * fname, std::ostream& os);
}