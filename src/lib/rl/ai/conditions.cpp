#include "conditions.h"
#include "blackboard.h"

#include <rl/components/components.h>
#include <core/math/norm.h>

namespace pgn
{
	namespace bt
	{
		void RegisterConditions()
		{
			cCondition::Register("HostilesDetected", &HostilesDetected);
			cCondition::Register("TargetInMeleeDistance", &TargetInMeleeDistance);
		}

		//------------------------------------------------------------------------------------------------------
		bool HostilesDetected(cBlackBoard& bb)
		{
			auto ptr = bb.mDictTemp.Get<std::vector<ecs::cEntityWithData>>("hostiles");
			return ptr ? !ptr->empty() : false;
		}

		//------------------------------------------------------------------------------------------------------
		bool TargetInMeleeDistance(cBlackBoard& bb)
		{
			return (*bb.mDictTemp.Get<float>("d2target")) < 2.0f;
		}
	}
}