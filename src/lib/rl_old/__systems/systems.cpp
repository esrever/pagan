#include "systems.h"
#include "ecs/SystemMgr.h"
#include "Door.h"
#include "KeyboardHandle.h"
#include "Log.h"
#include "MapWindow.h"
#include "Move.h"

namespace pgn
{
    void RegisterAllSystems(cSystemMgr& zMgr) 
	{
		zMgr.AddSystemType<pgn::sys::cDoor>();
		zMgr.AddSystemType<pgn::sys::cKeyboardHandle>();
		zMgr.AddSystemType<pgn::sys::cLog>();
		zMgr.AddSystemType<pgn::sys::cMapWindow>();
		zMgr.AddSystemType<pgn::sys::cMove>();
	}
}
