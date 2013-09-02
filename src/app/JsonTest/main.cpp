#include <iostream>
#include <fstream>
#include <sstream>
#include <pystring.h>
#include <boost/format.hpp>

#include <json_spirit.h>

#ifndef JSON_SPIRIT_VALUE_ENABLED
#error Please define JSON_SPIRIT_VALUE_ENABLED for the Value type to be enabled 
#endif


#include "../../lib/core/util/cmdopt.h"

/*
    I need to wrap this up, as well as the cmd opts, so that I can easily ask for an array or a value by get<int>("asd.qwe.etrty")
*/

using namespace std;

void process_level( const json_spirit::Value& val, int depth)
{
    string outp;
    ostringstream prefix;
    for(int i=0;i<depth;++i)
        prefix << "\t";
    switch( val.type())
    {
        case json_spirit::Value_type::obj_type:
            for( const auto &curpair : val.get_obj())
            {
                cout<<prefix.str() <<curpair.name_<< " : "<<endl;
                process_level( curpair.value_, depth+1);
            }
            break;
        case json_spirit::Value_type::array_type:
            for( const auto &curval : val.get_array())
            {
                process_level( curval, depth+1);
            }
            break;
        case json_spirit::Value_type::str_type:
            cout<<prefix.str() <<val.get_str()<<endl;
            break;
        case json_spirit::Value_type::bool_type:
            outp = "bool";
            break;
        case json_spirit::Value_type::int_type:
            outp = "int";
            break;
        case json_spirit::Value_type::real_type:
            outp = "real";
            break;
        case json_spirit::Value_type::null_type:
            outp = "null";
            break;
        default :
            outp = "BEEEEEP";
            break;
    }
}

int main(int argc, char ** argv)
{
    pgn::cCmdOpt cmdopts;
    cmdopts.Init(argc,argv);
    auto fjson = cmdopts.Opts().find("jopt");
    if( fjson != cmdopts.Opts().end())
    {
        const std::string& fname = fjson->second.front();
        ifstream is( fname );
        cout<<fname<<endl;
        json_spirit::Value root;
        json_spirit::read( is, root );
        is.close();
        process_level( root, 0);
    }
	return 0;
}