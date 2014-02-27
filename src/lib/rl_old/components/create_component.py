import sys

classname = sys.argv[1];

header = """
#pragma once

#include <core/util/json_conversions.h>

namespace pgn
{{
	//! TODO: describe
	struct c{0}
	{{
		
	}};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< c{0}>( c{0}& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< c{0}>( const c{0}& zData, JsonWriter& zRoot);
}}
""".format(classname);

source = """
#include "{0}.h"

namespace pgn
{{
	//-----------------------------------------------------------------------
	template<>
	bool from_json< c{0}>( c{0}& zData, const rapidjson::Value& zRoot)
	{{
		return true;
	}}

	//-----------------------------------------------------------------------
	template<>
	void to_json< c{0}>( const c{0}& zData, JsonWriter& zRoot)
	{{
		zRoot.StartObject();
		zRoot.EndObject();
	}}
}}
""".format(classname);

fp = open("{0}.cpp".format(classname), "w")
fp.write(source)
fp.close()

fp = open("{0}.h".format(classname), "w")
fp.write(header)
fp.close()