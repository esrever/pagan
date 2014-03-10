#include "conditions.h"

namespace pgn
{
	namespace bt
	{
		void RegisterConditions()
		{
			cCondition::Register("HostilesDetected", &HostilesDetected);
			cCondition::Register("TargetInMeleeDistance", &TargetInMeleeDistance);
		}
	}
}