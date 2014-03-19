#include "actions.h"
#include "blackboard.h"
#include <rlut/components/components.h>
#include <rlut/utils/shape/ShapeCalc.h>

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
			auto me = *bb.mDictTemp.Get<ecs::cEntityWithData>("me");
			auto tgt = *bb.mDictTemp.Get<ecs::cEntityWithData>("target");
			// TODO: request difi of target later.
			pgn::ecs::cmp::cLocation * me_loc = me->second.Component<pgn::ecs::cmp::cLocation>();
			pgn::ecs::cmp::cLocation * tgt_loc = tgt->second.Component<pgn::ecs::cmp::cLocation>();


			// make the distance function
			auto dfunc = [&](const glm::ivec2& off)->float{
				auto p = me_loc->mPos + off;
				// TODO: in range & walkable check
				bool in_range = true;
				if (in_range)
				{
					return pgn::norm_2(p - tgt_loc->mPos);
				}
				else
					return std::numeric_limits<float>::max();
			};

			// find the closest point to target
			auto iters = rlut::cShapeCalc< rlut::cBoxDistance>::Get(0, 1);
			auto best_it = iters.first;
			auto best_d = std::numeric_limits<float>::max();
			for (auto it = iters.first; it != iters.second; ++it)
			{
				auto d = dfunc(*it);
				if (d < best_d)
				{
					best_d = d;
					best_it = it;
				}
			}

			if (best_d == std::numeric_limits<float>::max())
				return eStatus::Failure;
			else
			{

				// TODO: MoveAdj action! use the iterator for the direction and the consumed MovePoints
				return eStatus::Success;
			}
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