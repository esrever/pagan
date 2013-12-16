#include "Door.h"

#include <ecs/Event.h>
#include <ecs/Component.h>
#include <ecs/EntityData.h>

#include <rl/components/Door.h>
#include <rl/components/MapWindow.h>


namespace pgn
{
	namespace sys
	{
		cDoor::cDoor()
			:mOnActionDoorOpen(Simple::slot(this, &cDoor::OnActionDoorOpen))
			,mOnActionDoorClose(Simple::slot(this, &cDoor::OnActionDoorClose))
			, mOnDoorOpened(Simple::slot(this, &cDoor::OnDoorOpened))
			, mOnDoorClosed(Simple::slot(this, &cDoor::OnDoorClosed))
		{

		}

		void cDoor::OnActionDoorOpen(cEntityWithData ewho, cEntityWithData ed1)
		{
			std::shared_ptr< cComponent<cmp::cDoor>> door_ptr;
			ed1->second.mComponents.GetComponent(door_ptr);
			if (door_ptr->mData.mIsClosed)
			{
				door_ptr->mData.mIsClosed = false;
				evt::cDoorOpened::mSig.emit(ed1);
			}
		}

		void cDoor::OnActionDoorClose(cEntityWithData ewho, cEntityWithData ed1)
		{
			std::shared_ptr< cComponent<cmp::cDoor>> door_ptr;
			ed1->second.mComponents.GetComponent(door_ptr);
			if (door_ptr->mData.mIsClosed)
			{
				door_ptr->mData.mIsClosed = true;
				evt::cDoorClosed::mSig.emit(ed1);
			}
		}

		void cDoor::OnDoorOpened(cEntityWithData ed)
		{
		}

		void cDoor::OnDoorClosed(cEntityWithData ed)
		{
		}
	}
}