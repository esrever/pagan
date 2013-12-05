#pragma once

#include <string>

#include <core/util/json_conversions.h>


namespace pgn
{
    namespace cmp
    {
    
        //! 
        struct cTextWindow
        {
			std::string mText;
			//oxygine Window // init with json config, provide nice defaults
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cTextWindow>( cmp::cTextWindow& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cTextWindow>( const cmp::cTextWindow& zData, JsonWriter& zRoot);
}