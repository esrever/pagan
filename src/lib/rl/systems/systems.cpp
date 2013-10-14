#include "systems.h"
#include "ecs/SystemMgr.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::cActionMappingSystem>();
		zMgr.AddSystemType<pgn::cAsciiRenderSystem>();
		zMgr.AddSystemType<pgn::cGameTurnSystem>();
	}
}
