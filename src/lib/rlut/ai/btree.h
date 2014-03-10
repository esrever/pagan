#pragma once

#include <memory>
#include "behavior.h"
#include "blackboard.h"

namespace pgn
{
	namespace bt
	{
		//! The core AI class of actors
		class cBehaviorTree
		{
		public:
			cBehaviorTree() :mRoot(nullptr){}

			cBehavior * Root() { return mRoot; }
			void SetRoot(cBehavior * p) { mRoot = p; }

			//! Run a single update on the tree
			eStatus Tick();

			//! RW access to temp data
			cBlackBoard& BlackBoard() { return mBlackBoard; }

		private:
			cBehavior * mRoot;
			cBlackBoard mBlackBoard;
		};
	}
}
