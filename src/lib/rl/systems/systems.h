#pragma once
#include "AsciiActionInputSystem.h"
#include "AsciiRenderSystem.h"
#include "GameTurnSystem.h"

namespace pgn
{
    class cSystemMgr;
    void RegisterAllSystems(cSystemMgr& zMgr);
}
