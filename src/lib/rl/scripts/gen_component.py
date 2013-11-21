import sys


srcdict = dict();

if len(sys.argv) < 3:
    print "Need 6 arguments: cmd name doxy [inch] [inccpp] [type,name,var]]"
    print sys.argv
    sys.exit(0);
else:
    for i in range(1,len(sys.argv)):
        [key, var] = sys.argv[i].split("=")
        srcdict[key] = var;
    
    if 'inc_h' in srcdict.keys():
        srcdict['includes_h'] = "\n".join(['#include ' + s for s in srcdict['inc_h'].split(',')])
    else: 
        srcdict['includes_h'] = '';
    if 'inc_cpp' in srcdict.keys():
        srcdict['includes_cpp'] = "\n".join(['#include ' + s for s in srcdict['inc_cpp'].split(',')])
    else: 
        srcdict['includes_cpp'] = '';
    
    vars = [];
    for key in srcdict:
        if key.startswith('var'):
            var = srcdict[key].split(',');
            vars.append(var);
    srcdict['vars_decl'] = "\n\t\t".join(['{0} {1};'.format(var[0],var[1]) for var in vars]);
    srcdict['from_json'] = "\n\t\t".join([ 'from_json( zData.{0}, zRoot["{1}"]);'.format(var[1],var[2]) for var in vars]);
    srcdict['to_json'] = "\n\t\t".join([ 'JsonWriter_AddMember("{0}", zData.{1}, zRoot);'.format(var[2],var[1]) for var in vars]);


src_h = """#pragma once

#include <core/util/json_conversions.h>
{includes_h}

namespace pgn
{{
	//! {doxy}
	struct c{class}
	{{
		{vars_decl}
	}};

	//-----------------------------------------------------------------------
	template<>
	bool from_json< c{class}>( c{class}& zData, const rapidjson::Value& zRoot);

	//-----------------------------------------------------------------------
	template<>
	void to_json< c{class}>( const c{class}& zData, JsonWriter& zRoot);
}}""".format(**srcdict)

src_cpp = """#include "{class}.h"
{includes_cpp}

namespace pgn
{{
	//----------------------------------------------------------------------------------
	template<>
	bool from_json<c{class}>( c{class}& zData, const rapidjson::Value& zRoot)
	{{
        {from_json}
        return true;
	}}

	//----------------------------------------------------------------------------------
	template<>
	void to_json<c{class}>( const c{class}& zData, JsonWriter& zRoot)
	{{
		zRoot.StartObject();
        {to_json}
		zRoot.EndObject();
	}}
}}""".format(**srcdict)

print src_h
print src_cpp

