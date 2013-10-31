#pragma once

#include "ActionMgrRL.h"
#include "actions/actions.h"

namespace pgn
{
	//-------------------------------------------------
	void cActionMgrRL::RegisterActionTypes()
	{
		RegisterAllActions(*this);
	}
}