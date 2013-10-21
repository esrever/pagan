#include "components.h"
#include "ecs/EntityMgr.h"

namespace pgn
{
    void RegisterAllComponents(cEntityMgr& zMgr) 
	{
		zMgr.AddComponentType<pgn::cAI>();
		zMgr.AddComponentType<pgn::cAsciiLevelRep>();
		zMgr.AddComponentType<pgn::cAsciiRep>();
		zMgr.AddComponentType<pgn::cDescription>();
		zMgr.AddComponentType<pgn::cInputKeyboard>();
		zMgr.AddComponentType<pgn::cInputMouse>();
		zMgr.AddComponentType<pgn::cLevel>();
		zMgr.AddComponentType<pgn::cLocation>();
		zMgr.AddComponentType<pgn::cLockable>();
		zMgr.AddComponentType<pgn::cMovement>();
		zMgr.AddComponentType<pgn::cOpenClose>();
		zMgr.AddComponentType<pgn::cPartOfWorld>();
		zMgr.AddComponentType<pgn::cPixelRep>();
		zMgr.AddComponentType<pgn::cTileAtmospheric>();
		zMgr.AddComponentType<pgn::cTileMoveCost>();
		zMgr.AddComponentType<pgn::cTileObstacle>();
		zMgr.AddComponentType<pgn::cTileStairs>();
		zMgr.AddComponentType<pgn::cTileVisibility>();
	}
}
