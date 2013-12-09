#include "components.h"
#include "ecs/EntityMgr.h"
#include "Description.h"
#include "KeyActionMapper.h"
#include "LevelPosition.h"
#include "LevelWindow.h"
#include "Log.h"
#include "MapSprite.h"
#include "MapWindow.h"
#include "Movement.h"
#include "OutStream.h"
#include "TextWindow.h"
#include "TileLayout.h"
#include "TileObstacle.h"

namespace pgn
{
    void RegisterAllComponents(cEntityMgr& zMgr) 
	{
		zMgr.AddComponentType<pgn::cmp::cDescription>();
		zMgr.AddComponentType<pgn::cmp::cKeyActionMapper>();
		zMgr.AddComponentType<pgn::cmp::cLevelPosition>();
		zMgr.AddComponentType<pgn::cmp::cLevelWindow>();
		zMgr.AddComponentType<pgn::cmp::cLog>();
		zMgr.AddComponentType<pgn::cmp::cMapSprite>();
		zMgr.AddComponentType<pgn::cmp::cMapWindow>();
		zMgr.AddComponentType<pgn::cmp::cMovement>();
		zMgr.AddComponentType<pgn::cmp::cOutStream>();
		zMgr.AddComponentType<pgn::cmp::cTextWindow>();
		zMgr.AddComponentType<pgn::cmp::cTileLayout>();
		zMgr.AddComponentType<pgn::cmp::cTileObstacle>();
	}
}
