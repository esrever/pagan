#include "systems.h"
#include "ecs/SystemMgr.h"
#include "KeyboardHandle.h"
#include "Log.h"
#include "Move.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::sys::cKeyboardHandle>();
		zMgr.AddSystemType<pgn::sys::cLog>();
		zMgr.AddSystemType<pgn::sys::cMove>();
	}
}
