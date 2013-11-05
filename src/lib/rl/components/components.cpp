#include "components.h"
#include "ecs/EntityMgr.h"
#include "AI.h"
#include "AsciiLevelRep.h"
#include "AsciiRep.h"
#include "AsciiWindow.h"
#include "Description.h"
#include "GameLog.h"
#include "GameStatus.h"
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
    void RegisterAllComponents(cEntityMgr& zMgr) 
	{
		zMgr.AddComponentType<pgn::cAI>();
		zMgr.AddComponentType<pgn::cAsciiLevelRep>();
		zMgr.AddComponentType<pgn::cAsciiRep>();
		zMgr.AddComponentType<pgn::cAsciiWindow>();
		zMgr.AddComponentType<pgn::cDescription>();
		zMgr.AddComponentType<pgn::cGameLog>();
		zMgr.AddComponentType<pgn::cGameStatus>();
		zMgr.AddComponentType<pgn::cKeyActionMapper>();
		zMgr.AddComponentType<pgn::cLevel>();
		zMgr.AddComponentType<pgn::cLocation>();
		zMgr.AddComponentType<pgn::cLockable>();
		zMgr.AddComponentType<pgn::cMouseActionMapper>();
		zMgr.AddComponentType<pgn::cMovement>();
		zMgr.AddComponentType<pgn::cOpenClose>();
		zMgr.AddComponentType<pgn::cPixelRep>();
		zMgr.AddComponentType<pgn::cTileAtmospheric>();
		zMgr.AddComponentType<pgn::cTileMoveCost>();
		zMgr.AddComponentType<pgn::cTileObstacle>();
		zMgr.AddComponentType<pgn::cTileStairs>();
		zMgr.AddComponentType<pgn::cTileVisibility>();
	}
}
