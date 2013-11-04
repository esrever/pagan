#pragma once
#include "AsciiActionInputSystem.h"
#include "AsciiLogRenderSystem.h"
#include "AsciiMapRenderSystem.h"
#include "AsciiRenderSystem.h"
#include "AsciiStatusRenderSystem.h"
#include "GameTurnSystem.h"

namespace pgn
{
    class cSystemMgr;
    void RegisterAllSystems(cSystemMgr& zMgr);
}
