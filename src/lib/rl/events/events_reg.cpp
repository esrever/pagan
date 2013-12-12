#include "events.h"
#include <rl/SystemMgrRL.h>


namespace pgn
{
	void cSystemMgrRL::RegisterAllEvents()
	{
		evt::cLevelCreated::mSig += &evt::OnLevelCreated;
		evt::cLevelDestroy::mSig += &evt::OnLevelDestroy;
		evt::cLevelLoaded::mSig += &evt::OnLevelLoaded;
		evt::cLevelUnload::mSig += &evt::OnLevelUnload;
		evt::cLevelEntered::mSig += &evt::OnLevelEntered;
		evt::cLevelLeave::mSig += &evt::OnLevelLeave;
		evt::cExitApplication::mSig += &evt::OnExitApplication;
		evt::cLog::mSig += &evt::OnLog;
		evt::cActionIdle::mSig += &evt::OnActionIdle;
		evt::cActionMoveAdj::mSig += &evt::OnActionMoveAdj;
		evt::cActionDoorOpen::mSig += &evt::OnActionDoorOpen;
		evt::cActionDoorClose::mSig += &evt::OnActionDoorClose;
		evt::cDoorOpened::mSig += &evt::OnDoorOpened;
		evt::cDoorClosed::mSig += &evt::OnDoorClosed;
		evt::cTileInLevelChanged::mSig += &evt::OnTileInLevelChanged;
        evt::cKeyState::mSig += &evt::OnKeyState;
	}
}