#include "btree.h"
#include "behavior.h"

#include <rl/systems/UpdateAI.h>

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
		std::string preset;
		SerializeIn(node, "preset", preset);
		auto ptr = mainecs()->System<ecs::sys::cUpdateAI>().GetBehavior(preset);;
		value.SetRoot(ptr);
		return ptr ? 1 : 0;
	}
}