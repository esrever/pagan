#include "systems.h"
#include "ecs/SystemMgr.h"
#include "KeyboardHandle.h"
#include "Log.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::sys::cKeyboardHandle>();
		zMgr.AddSystemType<pgn::sys::cLog>();
	}
}
