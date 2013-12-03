#include "xml_conversions.h"

namespace pgn
{
	std::shared_ptr<pugi::xml_document> file_to_xml(const std::string& zFname)
	{
		auto ret = std::make_shared<pugi::xml_document>();
		ret->load_file(zFname.c_str());
		return ret;
	}
}