#pragma once
#include "AsciiRenderSystem.h"
#include "GameTurnSystem.h"
#include "MapInputSystem.h"

namespace pgn
{
    class cSystemMgr;
    void RegisterAllSystems(cSystemMgr& zMgr);
}
