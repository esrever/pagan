#include "actions.h"

namespace pgn
{
	namespace bt
	{
		void RegisterActions()
		{
			cAction::Register("ApproachTarget",&ApproachTarget);
			cAction::Register("Wander", &Wander);
			cAction::Register("MeleeAttackTarget", &MeleeAttackTarget);
			cAction::Register("SelectTarget", &SelectTarget);
			cAction::Register("SenseHostiles", &SenseHostiles);
			cAction::Register("DistanceToTarget", &DistanceToTarget);
		}
	}
}