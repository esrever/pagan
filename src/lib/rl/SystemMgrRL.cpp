#pragma once

#include "SystemMgrRL.h"
#include "systems/systems.h"

namespace pgn
{
	void cSystemMgrRL::RegisterSystemTypes()
	{
		RegisterAllSystems(*this);
	}
}