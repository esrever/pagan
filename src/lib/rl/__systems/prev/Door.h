#pragma once

#include <ecs/SystemBase.h>

#include <rl/events/events.h>

namespace pgn
{
	namespace cmp
	{
	}

	namespace sys
	{
		class cDoor : public cSystemBase
		{
		public:
			cDoor();

		private:
			cEventHandler<evt::cActionDoorOpen> mOnActionDoorOpen;
			cEventHandler<evt::cActionDoorClose> mOnActionDoorClose;
			cEventHandler<evt::cDoorOpened> mOnDoorOpened;
			cEventHandler<evt::cDoorClosed> mOnDoorClosed;
		private:
			//! Receiving functions
			void OnActionDoorOpen(cEntityWithData, cEntityWithData);
			void OnActionDoorClose(cEntityWithData, cEntityWithData);
			void OnDoorOpened(cEntityWithData);
			void OnDoorClosed(cEntityWithData);

		private:

			//! Find all Moves
			cQueryExpressionSptr mQuery;
		};
	}
}