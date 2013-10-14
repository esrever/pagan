#pragma once
#include "ActionMappingSystem.h"
#include "AsciiRenderSystem.h"
#include "GameTurnSystem.h"

namespace pgn
{
    class cSystemMgr;
    void RegisterAllSystems(cSystemMgr& zMgr);
}
