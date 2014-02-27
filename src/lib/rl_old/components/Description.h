#pragma once

#include <core/util/json_conversions.h>
#include <string>

namespace pgn
{
    namespace cmp
    {
    
        //! Info about the entity
        struct cDescription
        {
            std::string mVerbose;
		std::string mShort;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cDescription>( cmp::cDescription& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cDescription>( const cmp::cDescription& zData, JsonWriter& zRoot);
}