#include "systems.h"
#include "ecs/SystemMgr.h"
#include "LogSystem.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::sys::cLog>();
	}
}
