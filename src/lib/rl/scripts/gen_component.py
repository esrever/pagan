import sys


srcdict = dict();

if len(sys.argv) <= 6:
    print "Need 6 arguments: cmd name doxy [inch] [inccpp] [type,name,var]]"
    sys.exit(0);
else:
    srcdict['name'] = sys.argv[1];
    srcdict['desc'] = sys.argv[2];
    srcdict['includes_h'] = "\n".join(['#include ' + s for s in sys.argv[3].split(',')])
    srcdict['includes_cpp'] = "\n".join(['#include ' + s for s in sys.argv[4].split(',')])
    
    vars = [];
    for i in range(5,len(sys.argv)):
        var = sys.argv[i].split(',');
        vars.append(var);
    srcdict['vars_decl'] = "\n\t\t".join(['{0} {1};'.format(var[0],var[1]) for var in vars]);
    srcdict['from_json'] = "\n\t\t".join([ 'from_json( zData.{0}, zRoot["{1}"]);'.format(var[1],var[2]) for var in vars]);
    srcdict['to_json'] = "\n\t\t".join([ 'JsonWriter_AddMember("{0}", zData.{1}, zRoot);'.format(var[2],var[1]) for var in vars]);


src_h = """#pragma once

#include <core/util/json_conversions.h>
{includes_h}

namespace pgn
{{
	//! {desc}
	struct c{name}
	{{
		{vars_decl}
	}};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< c{name}>( c{name}& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< c{name}>( const c{name}& zData, JsonWriter& zRoot);
}}""".format(**srcdict)

src_cpp = """#include "{name}.h"
{includes_cpp}

namespace pgn
{{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<c{name}>( c{name}& zData, const rapidjson::Value& zRoot)
	{{
        {from_json}
        return true;
	}}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<c{name}>( const c{name}& zData, JsonWriter& zRoot)
	{{
		zRoot.StartObject();
        {to_json}
		zRoot.EndObject();
	}}
}}""".format(**srcdict)

print src_h
print src_cpp

