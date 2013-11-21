#include "components.h"
#include "ecs/EntityMgr.h"
#include "KeyActionMapper.h"

namespace pgn
{
    void RegisterAllComponents(cEntityMgr& zMgr) 
	{
		zMgr.AddComponentType<pgn::cKeyActionMapper>();
	}
}
