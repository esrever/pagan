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
		class cMapWindow : public cSystemBase
		{
		public:
			cMapWindow();

		private:
			cEventHandler<evt::cLevelLoaded> mOnLevelLoaded;
			cEventHandler<evt::cLevelUnload> mOnLevelUnload;
		private:
			//! Receiving functions
			void OnLevelLoaded(cEntityWithData);
			void OnLevelUnload(cEntityWithData);

		private:

			//! Find all Levels
			cQueryExpressionSptr mQueryMapWindow;
		};
	}
}