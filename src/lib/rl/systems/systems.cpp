#include "systems.h"
#include "ecs/SystemMgr.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::cAsciiRenderSystem>();
	}
}
