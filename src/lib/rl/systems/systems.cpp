#include "systems.h"
#include "ecs/SystemMgr.h"
#include "Log.h"
#include "LogDisplay.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::sys::cLog>();
		zMgr.AddSystemType<pgn::sys::cLogDisplay>();
	}
}
