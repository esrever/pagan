#pragma once
#include "AI.h"
#include "AsciiLevelRep.h"
#include "AsciiRep.h"
#include "AsciiWindow.h"
#include "Description.h"
#include "GameLog.h"
#include "KeyActionMapper.h"
#include "Level.h"
#include "Location.h"
#include "Lockable.h"
#include "MouseActionMapper.h"
#include "Movement.h"
#include "OpenClose.h"
#include "PixelRep.h"
#include "TileAtmospheric.h"
#include "TileMoveCost.h"
#include "TileObstacle.h"
#include "TileStairs.h"
#include "TileVisibility.h"

namespace pgn
{
    class cEntityMgr;
    void RegisterAllComponents(cEntityMgr& zMgr);
}
