#pragma once

#include <core/util/json_conversions.h>
#include <vector>
#include <string>

namespace pgn
{
    namespace cmp
    {
    
        //! Stores logs
        struct cLog
        {
            std::vector<std::string> mLines;
			unsigned				 mMaxLineNum;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cLog>( cmp::cLog& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cLog>( const cmp::cLog& zData, JsonWriter& zRoot);
}