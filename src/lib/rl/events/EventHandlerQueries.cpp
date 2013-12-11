#include "EventHandlerQueries.h"


namespace pgn
{
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_LevelCreated::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_LevelDestroy::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_LevelLoaded::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_LevelUnload::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_ExitApplication::Handle( void )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_Log::Handle( const std::string&, const std::string& )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_ActionIdle::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_ActionMoveAdj::Handle( cEntityWithData, const glm::ivec2& )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_ActionDoorOpen::Handle( cEntityWithData, cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_ActionDoorClose::Handle( cEntityWithData, cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_DoorOpened::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_DoorClosed::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
	//----------------------------------------------------------------------------------------------------
	void cEventHandlerQuery_TileInLevelChanged::Handle( cEntityWithData )
	{
		if (!mQuery) return;
		assert(false);
	}
	
}