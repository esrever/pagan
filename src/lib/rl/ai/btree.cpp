#include "btree.h"
#include "behavior.h"

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

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const bt::cBehaviorTree& value)
	{
		
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, bt::cBehaviorTree& value)
	{
		size_t ret = 0;
		bt::cBehavior * behavior;
		ret += bt::cBehavior::SerializeIn(node, behavior);
		value.SetRoot(behavior);
		return ret;
	}
}