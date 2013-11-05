#include "systems.h"
#include "ecs/SystemMgr.h"
#include "AsciiActionInputSystem.h"
#include "AsciiLogRenderSystem.h"
#include "AsciiMapRenderSystem.h"
#include "AsciiStatusRenderSystem.h"
#include "GameTurnSystem.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::cAsciiActionInputSystem>();
		zMgr.AddSystemType<pgn::cAsciiLogRenderSystem>();
		zMgr.AddSystemType<pgn::cAsciiMapRenderSystem>();
		zMgr.AddSystemType<pgn::cAsciiStatusRenderSystem>();
		zMgr.AddSystemType<pgn::cGameTurnSystem>();
	}
}
