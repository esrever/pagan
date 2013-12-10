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
			virtual void Process();
			virtual bool from_json(const rapidjson::Value& zRoot);
			virtual void to_json(JsonWriter& zRoot) const;

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