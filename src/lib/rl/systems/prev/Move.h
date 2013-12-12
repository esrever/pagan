#pragma once

#include <ecs/SystemBase.h>

#include <rl/events/events.h>

namespace pgn
{
	namespace cmp
	{
		struct cMovement;
	}

	namespace sys
	{
		class cMove : public cSystemBase
		{
		public:
			cMove();

		private:
			cEventHandler<evt::cActionMoveAdj> mOnActionMoveAdj;
			cEventHandler<evt::cActionIdle> mOnActionIdle;
		private:
			//! Receiving functions
			void OnActionIdle(cEntityWithData);
			void OnActionMoveAdj(cEntityWithData, const glm::ivec2&);

		private:

			//! Find all Moves
			cQueryExpressionSptr mQueryLog;
		};
	}
}