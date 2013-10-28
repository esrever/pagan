#include "components.h"
#include "ecs/EntityMgr.h"

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
