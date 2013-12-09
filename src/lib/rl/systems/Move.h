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
			virtual void Process();
			virtual bool from_json(const rapidjson::Value& zRoot);
			virtual void to_json(JsonWriter& zRoot) const;

		private:
			cEventHandler<evt::cActionMoveAdj> mOnActionMoveAdj;
			cEventHandler<evt::cActionIdle> mOnActionIdle;
		private:
			//! Receiving functions
			void OnActionIdle(cEntity);
			void OnActionMoveAdj(cEntity, const glm::ivec2&);

		private:

			//! Find all Moves
			cQueryExpressionSptr mQueryLog;
		};
	}
}