#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
	//! Type of entity wrt map
	enum class eMapType
	{
		MapBackground, // floor, wall, etc
		MapObject,	   // altars, statues, etc
		Item,		   // swords, wands, etc.
		Creature,	   // rats, humans, etc.
		Effect,		   // spells, arrows, fire, etc
	};

    namespace cmp
    {
    
        //! Move traits
        struct cWorldTraits
        {
			eMapType mMapType;
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
	bool from_json< cmp::cWorldTraits>(cmp::cWorldTraits& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
	void to_json< cmp::cWorldTraits>(const cmp::cWorldTraits& zData, JsonWriter& zRoot);
}