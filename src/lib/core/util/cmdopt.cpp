#include "cmdopt.h"
#include <pystring.h>

#include <iostream>

namespace pgn
{
    void cCmdOpt::Init(int argc, char ** argv)
    {
        for(int i=1;i<argc;++i)
        {
            std::string arg = argv[i];
            
            std::vector<std::string> strings;

			// Looking for format a=b
            pystring::split( arg, strings, "=", 1);
            
            //Failed: less than 2 items
            if(strings.size() < 2) continue;
            
            const std::string key = strings[0];
            const std::string val = strings[1];
            
            //Failed: found =a or a=
            if (key.empty() || val.empty()) continue;
            
			// Now extract the vector of values
            pystring::split( val, strings, ",");
            
            // a=b0,b1,b2,...,bn format is invalid
            bool invalid = false;
            for(auto s:strings ) if (s.empty()) invalid = true;
            if(invalid) continue;
            
            mCfgData.Set(key,strings);
        }
    }
}