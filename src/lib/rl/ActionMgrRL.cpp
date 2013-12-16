#pragma once

#include "ActionMgrRL.h"
#include "events/actions.h"

namespace pgn
{
	//-------------------------------------------------
	void cActionMgrRL::RegisterActionTypes()
	{
		RegisterAllActions(*this);
	}
}