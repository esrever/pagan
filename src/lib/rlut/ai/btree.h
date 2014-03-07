#pragma once

#include <memory>
#include <core/container/dict.h>
#include "behavior.h"

namespace pgn
{
	namespace bt
	{
		class cBehaviorTree
		{
		public:
			cBehaviorTree() :mRoot(nullptr){}


			cBehavior * Root() { return mRoot; }
			void SetRoot(cBehavior * p);

			//! TODO: Later, instead of passing dict, pass a blackboard object, which is the dict plus actor and world (as we're gonna use both a lot)
			eStatus Tick() { return mRoot->Tick(mDict); }
			cDict& Dict() { return mDict; }

		private:
			cBehavior * mRoot;
			cDict mDict;
		};
	}
}
