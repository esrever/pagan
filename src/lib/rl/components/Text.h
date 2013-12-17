#pragma once

#include <core/util/json_conversions.h>
#include <string>

namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cText
        {
            std::string mText;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cText>( cmp::cText& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cText>( const cmp::cText& zData, JsonWriter& zRoot);
}