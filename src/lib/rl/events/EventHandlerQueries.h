#pragma once

#include <ecs/Event.h>
#include <ecs/QueryExpression.h>
#include <ecs/EventHandlerQuery.h>
#include <rl/events/events.h>


namespace pgn
{
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_LevelCreated : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_LevelCreated():mHandler(Simple::slot(this, &cEventHandlerQuery_LevelCreated::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelCreated>(*this));}
	private:
		cEventHandler<evt::cLevelCreated> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_LevelDestroy : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_LevelDestroy():mHandler(Simple::slot(this, &cEventHandlerQuery_LevelDestroy::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelDestroy>(*this));}
	private:
		cEventHandler<evt::cLevelDestroy> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_LevelLoaded : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_LevelLoaded():mHandler(Simple::slot(this, &cEventHandlerQuery_LevelLoaded::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelLoaded>(*this));}
	private:
		cEventHandler<evt::cLevelLoaded> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_LevelUnload : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_LevelUnload():mHandler(Simple::slot(this, &cEventHandlerQuery_LevelUnload::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_LevelUnload>(*this));}
	private:
		cEventHandler<evt::cLevelUnload> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_ExitApplication : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_ExitApplication():mHandler(Simple::slot(this, &cEventHandlerQuery_ExitApplication::Handle)){}
		void Handle( void );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ExitApplication>(*this));}
	private:
		cEventHandler<evt::cExitApplication> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_Log : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_Log():mHandler(Simple::slot(this, &cEventHandlerQuery_Log::Handle)){}
		void Handle( const std::string&, const std::string& );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_Log>(*this));}
	private:
		cEventHandler<evt::cLog> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_ActionIdle : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_ActionIdle():mHandler(Simple::slot(this, &cEventHandlerQuery_ActionIdle::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionIdle>(*this));}
	private:
		cEventHandler<evt::cActionIdle> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_ActionMoveAdj : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_ActionMoveAdj():mHandler(Simple::slot(this, &cEventHandlerQuery_ActionMoveAdj::Handle)){}
		void Handle( cEntityWithData, const glm::ivec2& );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionMoveAdj>(*this));}
	private:
		cEventHandler<evt::cActionMoveAdj> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_ActionDoorOpen : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_ActionDoorOpen():mHandler(Simple::slot(this, &cEventHandlerQuery_ActionDoorOpen::Handle)){}
		void Handle( cEntityWithData, cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionDoorOpen>(*this));}
	private:
		cEventHandler<evt::cActionDoorOpen> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_ActionDoorClose : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_ActionDoorClose():mHandler(Simple::slot(this, &cEventHandlerQuery_ActionDoorClose::Handle)){}
		void Handle( cEntityWithData, cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_ActionDoorClose>(*this));}
	private:
		cEventHandler<evt::cActionDoorClose> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_DoorOpened : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_DoorOpened():mHandler(Simple::slot(this, &cEventHandlerQuery_DoorOpened::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_DoorOpened>(*this));}
	private:
		cEventHandler<evt::cDoorOpened> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_DoorClosed : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_DoorClosed():mHandler(Simple::slot(this, &cEventHandlerQuery_DoorClosed::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_DoorClosed>(*this));}
	private:
		cEventHandler<evt::cDoorClosed> mHandler;
	};
	
	//----------------------------------------------------------------------------------------------------
	class cEventHandlerQuery_TileInLevelChanged : public cEventHandlerQueryBase
	{
	public:
		cEventHandlerQuery_TileInLevelChanged():mHandler(Simple::slot(this, &cEventHandlerQuery_TileInLevelChanged::Handle)){}
		void Handle( cEntityWithData );
	virtual std::shared_ptr<cEventHandlerQueryBase> clone() const {return std::dynamic_pointer_cast<cEventHandlerQueryBase>(std::make_shared<cEventHandlerQuery_TileInLevelChanged>(*this));}
	private:
		cEventHandler<evt::cTileInLevelChanged> mHandler;
	};
}