#include "components.h"
#include "ecs/EntityMgr.h"
#include "KeyActionMapper.h"
#include "TileObstacle.h"

namespace pgn
{
    void RegisterAllComponents(cEntityMgr& zMgr) 
	{
		zMgr.AddComponentType<pgn::cKeyActionMapper>();
		zMgr.AddComponentType<pgn::cTileObstacle>();
	}
}
