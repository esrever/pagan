#include "btree.h"

namespace pgn
{
	namespace bt
	{
		eStatus cBehaviorTree::Tick() 
		{ 
			mBlackBoard.mDictTemp = cDict(); 
			return mRoot->Tick(mBlackBoard); 
		}
	}
}