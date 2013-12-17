#pragma once

#include <string>
#include <core/util/json_conversions.h>

#include <rl/util/FormatString.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cFormatString
        {
            std::string mText;
			cDict		mDict;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cFormatString>( cmp::cFormatString& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cFormatString>( const cmp::cFormatString& zData, JsonWriter& zRoot);
}