#pragma once

#include <ecs/SystemBase.h>

#include <rl/events/events.h>
#include <rl/oxygine/KeyboardMgr.h>

namespace pgn
{
	namespace sys
	{
		class cKeyboardHandle : public cSystemBase
		{
		public:
			cKeyboardHandle();

		private:
			cEventHandler<evt::cKey> mOnKey;
		private:
			//! Receiving functions
			void OnKey(const int, const oxygine::cKeyState&);

		private:

			//! Find all entities that handle key presses
			cQueryExpressionSptr mQueryKeyActionMapper;

			//! Find all dev consoles (should be one, really)
			cQueryExpressionSptr mQueryDevConsole;
		};
	}
}