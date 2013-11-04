#include "systems.h"
#include "ecs/SystemMgr.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::cAsciiActionInputSystem>();
		zMgr.AddSystemType<pgn::cAsciiLogRenderSystem>();
		zMgr.AddSystemType<pgn::cAsciiMapRenderSystem>();
		zMgr.AddSystemType<pgn::cAsciiRenderSystem>();
		zMgr.AddSystemType<pgn::cAsciiStatusRenderSystem>();
		zMgr.AddSystemType<pgn::cGameTurnSystem>();
	}
}
