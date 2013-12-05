#include "systems.h"
#include "ecs/SystemMgr.h"
#include "Log.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::sys::cLog>();
	}
}
