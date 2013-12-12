#pragma once

#include "SystemMgrRL.h"
#include <ecs/SystemBase.h>

namespace pgn
{
	cSystemMgrRL::cSystemMgrRL()
	:mOnExitApplication(Simple::slot(this, &cSystemMgrRL::OnExitApplication))
	{}

	//-------------------------------------------------
	bool cSystemMgrRL::UpdateFrame()
	{
		/* TODO: 
			TimeKeeperSystem
			ActionMappingSystem
			GameTurnSystem
			AsciiRenderSystem
		*/
		mLoop = true;
		cEvent < size_t(evt::eBasicECS::UPDATE)>::mSig.emit();
		// TODO: on update
		return mLoop;
	}


	void cSystemMgrRL::OnExitApplication()
	{
		mLoop=false;
	}
}