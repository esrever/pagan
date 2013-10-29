#include "systems.h"
#include "ecs/SystemMgr.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::cAsciiActionInputSystem>();
		zMgr.AddSystemType<pgn::cAsciiRenderSystem>();
		zMgr.AddSystemType<pgn::cGameTurnSystem>();
		zMgr.AddSystemType<pgn::cMapInputSystem>();
	}
}
