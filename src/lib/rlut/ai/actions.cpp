#include "actions.h"
#include "blackboard.h"
#include <rlut/components/components.h>

namespace pgn
{
	namespace bt
	{
		//------------------------------------------------------------------------------------------------------
		void RegisterActions()
		{
			cAction::Register("ApproachTarget",&ApproachTarget);
			cAction::Register("Wander", &Wander);
			cAction::Register("MeleeAttackTarget", &MeleeAttackTarget);
			cAction::Register("SelectTarget", &SelectTarget);
			cAction::Register("SenseHostiles", &SenseHostiles);
			cAction::Register("DistanceToTarget", &DistanceToTarget);
		}

		//------------------------------------------------------------------------------------------------------
		eStatus ApproachTarget(cBlackBoard& bb)
		{
			auto me = *bb.mDictTemp.Get<cECS::cEntityWithData>("me");
			auto tgt = *bb.mDictTemp.Get<cECS::cEntityWithData>("target");
			// TODO: request difi of target later.
			pgn::rl::cmp::cLocation * me_loc = me->second.Component<pgn::rl::cmp::cLocation>();
			pgn::rl::cmp::cLocation * tgt_loc = tgt->second.Component<pgn::rl::cmp::cLocation>();

			return eStatus::Failure;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus Wander(cBlackBoard&)
		{
			return eStatus::Failure;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus MeleeAttackTarget(cBlackBoard&)
		{
			return eStatus::Failure;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus SelectTarget(cBlackBoard&)
		{
			return eStatus::Failure;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus SenseHostiles(cBlackBoard&)
		{
			return eStatus::Failure;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus DistanceToTarget(cBlackBoard&)
		{
			return eStatus::Failure;
		}

		//------------------------------------------------------------------------------------------------------
	}
}