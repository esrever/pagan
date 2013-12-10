#pragma once

#include <oxygine-framework.h>
#include <core/util/json_conversions.h>

#include "MapSprite.h"


namespace pgn
{
    namespace cmp
    {
    
        //! TODO: later add locking component
        struct cDoor
        {
			bool mIsClosed;
			cMapSprite mSprites[2];
        };
        
    }

    //-----------------------------------------------------------------------
    template<>
	bool from_json< cmp::cDoor>(cmp::cDoor& zData, const rapidjson::Value& zRoot);
    //-----------------------------------------------------------------------
    template<>
	void to_json< cmp::cDoor>(const cmp::cDoor& zData, JsonWriter& zRoot);

	namespace evt
	{

	}

	/*
		OnOpenDoor(e0 who, e1 door) : check open/close status, check if locked, blocked, etc
		OnCloseDoor(e0 who, e1 door) : check open/close status, check if blocked, etc
		OnLockDoor(e0 who, e1 door) : check on locked status, etc
		OnUnlockDoor(e0 who, e1 door) : check on locked status, etc
		OnDoorOpened(e0 door) : change mapsprite, change tile obstacle
		OnDoorClosed(e0 door) : change mapsprite, change tile obstacle
	*/
}