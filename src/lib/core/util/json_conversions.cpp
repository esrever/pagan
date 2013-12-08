#include "json_conversions.h"

#include <boost/format.hpp>
#include <cassert>

namespace pgn
{
	std::shared_ptr<rapidjson::Document> file_to_json(const std::string& zFname)
	{
		auto doc = std::shared_ptr<rapidjson::Document>( new rapidjson::Document());
		const std::string text = file_to_text(zFname);
		if(doc->Parse<0>(text.c_str()).HasParseError())
		{
			std::string text_0 = text.substr(0, doc->GetErrorOffset());
			auto errs = boost::str(boost::format( "Json Error(%s): %s\n%s\n") % zFname.c_str() % doc->GetParseError() % text_0.c_str());
			std::cerr<< errs<< std::endl;
		}
		return doc;
	}
}