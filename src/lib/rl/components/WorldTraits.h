#pragma once

#include <core/util/json_conversions.h>


namespace pgn
{
	//! Type of entity wrt map
	enum eMapType
	{
		MapBackground, // floor, wall, etc
		MapObject,	   // altars, statues, etc
		Item,		   // swords, wands, etc.
		Creature,	   // rats, humans, etc.
		Effect,		   // spells, arrows, fire, etc
	};

	DECL_ENUMSTR(eMapType, MapBackground);
	DECL_ENUMSTR(eMapType, MapObject);
	DECL_ENUMSTR(eMapType, Item);
	DECL_ENUMSTR(eMapType, Creature);
	DECL_ENUMSTR(eMapType, Effect);

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