#pragma once

#include <core/util/json_conversions.h>

#include <rl/oxygine/window.h>


namespace pgn
{
    namespace cmp
    {
    
		//! 
		struct cLevelWindow
		{
			cWindow mWindow;
		};
        
    }

    //-----------------------------------------------------------------------
    template<>
    bool from_json< cmp::cLevelWindow>( cmp::cLevelWindow& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
    void to_json< cmp::cLevelWindow>( const cmp::cLevelWindow& zData, JsonWriter& zRoot);
}