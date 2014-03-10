#include "conditions.h"
#include "blackboard.h"
#include <rlut/components/components.h>

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
			return false;
		}

		//------------------------------------------------------------------------------------------------------
		bool TargetInMeleeDistance(cBlackBoard& bb)
		{
			return false;
		}
	}
}