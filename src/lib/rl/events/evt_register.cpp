#include "events.h"
#include <rl/SystemMgrRL.h>
#include <ecs/EventHandlerQuery.h>
#include "EventHandlerQueries.h"
namespace pgn
{
	void cSystemMgrRL::RegisterEventHandlers()
	{
		mAllEventHandlerQueries["LevelCreated"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelCreated>());
		mAllEventHandlerQueries["LevelDestroy"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelDestroy>());
		mAllEventHandlerQueries["LevelLoaded"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelLoaded>());
		mAllEventHandlerQueries["LevelUnload"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelUnload>());
		mAllEventHandlerQueries["ExitApplication"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ExitApplication>());
		mAllEventHandlerQueries["Log"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_Log>());
		mAllEventHandlerQueries["ActionIdle"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionIdle>());
		mAllEventHandlerQueries["ActionMoveAdj"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionMoveAdj>());
		mAllEventHandlerQueries["ActionDoorOpen"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionDoorOpen>());
		mAllEventHandlerQueries["ActionDoorClose"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionDoorClose>());
		mAllEventHandlerQueries["DoorOpened"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_DoorOpened>());
		mAllEventHandlerQueries["DoorClosed"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_DoorClosed>());
		mAllEventHandlerQueries["TileInLevelChanged"]= std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_TileInLevelChanged>());
	}
}