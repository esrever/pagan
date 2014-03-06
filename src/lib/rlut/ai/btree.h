#pragma once

#include <memory>
#include <core/container/dict.h>
#include "behavior.h"

namespace pgn
{
	class cBehaviorTree
	{
	public:
		cBehaviorTree() :mRoot(nullptr){}
		
		
		Behavior * Root() { return mRoot; }
		void SetRoot(Behavior * p);

		//! TODO: Later, instead of passing dict, pass a blackboard object, which is the dict plus actor and world (as we're gonna use both a lot)
		eBehaviorStatus Tick() { return mRoot->Tick(mDict); }
		cDict& Dict() { return mDict; }

	private:
		Behavior * mRoot;
		cDict mDict;
	};
}
