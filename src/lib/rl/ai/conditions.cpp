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
			cCondition::Register("CriticalHealth", &CriticalHealth);
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

		//------------------------------------------------------------------------------------------------------
		bool CriticalHealth(cBlackBoard& bb)
		{
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			const auto& stats = me->second.Component<ecs::cmp::cCreature>()->mCreatureData;
			// TODO: critical health should be a parameter
			const float critical_health = 0.991f;
			return (stats.mCurHealth / float(stats.mMaxHealth)) < critical_health;
		}
	}
}