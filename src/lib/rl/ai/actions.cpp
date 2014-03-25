#include "actions.h"
#include "blackboard.h"

#include <ecs/ecs.h>
#include <rl/components/components.h>
#include <rl/systems/MoveAdj.h>
#include <rl/tile/layout.h>
#include <rl/utils/shape/ShapeCalc.h>

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
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			auto tgt = *bb.mDictTemp.Get<ecs::cEntityWithData>("target");
			// TODO: request difi of target later.
			pgn::ecs::cmp::cLocation * me_loc = me->second.Component<pgn::ecs::cmp::cLocation>();
			pgn::ecs::cmp::cLocation * tgt_loc = tgt->second.Component<pgn::ecs::cmp::cLocation>();

			const auto& world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
			const auto& lvl = world->mLevelMap.find(me_loc->mLevelId)->second;
			const auto& layout = lvl->second.Component<ecs::cmp::cLevelData>()->mLayout;


			// make the distance function
			auto dfunc = [&](const glm::ivec2& off)->float{
				auto p = me_loc->mPos + off;
				// TODO: in range & walkable check
				bool in_range = true;
				if (in_range && (!layout.Obstacles()(p)))
				{
					return pgn::norm_2(p - tgt_loc->mPos);
				}
				else
					return std::numeric_limits<float>::max();
			};

			// find the closest point to target
			auto iters = rl::cShapeCalc< rl::cBoxDistance>::Get(0, 1);
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
				mainecs()->System<ecs::sys::cMoveAdj>()(me, *best_it);
				return eStatus::Success;
			}
		}

		//------------------------------------------------------------------------------------------------------
		eStatus Wander(cBlackBoard& bb)
		{
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			bool ok = mainecs()->System<ecs::sys::cMoveAdj>()(me, glm::ivec2((rand() % 3) - 1, (rand() % 3) - 1));
			return eStatus::Success;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus MeleeAttackTarget(cBlackBoard&)
		{
			// TODO: replace wuth something more appropriate
			std::cout << "Squeeeeaaaak!!!!" << std::endl; 
			return eStatus::Success;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus SelectTarget(cBlackBoard& bb)
		{
			// TODO: add logic on who's the target
			const auto& hostiles = bb.mDictTemp.Get<std::vector<ecs::cEntityWithData>>("hostiles");
			bb.mDictTemp.Insert("target",hostiles->front());
			return eStatus::Success;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus SenseHostiles(cBlackBoard& bb)
		{
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			const auto& me_pos = me->second.Component<ecs::cmp::cLocation>()->mPos;
			auto hero = pgn::mainecs()->TagusToEntities("Player");
			const auto& hero_pos = hero->second->second.Component<ecs::cmp::cLocation>()->mPos;

			// TODO: read LoS from settings
			const float los = 114.0f;
			if (pgn::norm_2(me_pos- hero_pos) < los)
			{
				std::vector<ecs::cEntityWithData> hostiles;
				hostiles.push_back(hero->second);
				bb.mDictTemp.Insert("hostiles", hostiles);
			}
			return eStatus::Success;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus DistanceToTarget(cBlackBoard& bb)
		{
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			auto tgt = *bb.mDictTemp.Get<ecs::cEntityWithData>("target");
			const auto& me_pos = me->second.Component<pgn::ecs::cmp::cLocation>()->mPos;
			const auto& tgt_pos = tgt->second.Component<pgn::ecs::cmp::cLocation>()->mPos;
			bb.mDictTemp.Insert<float>("d2target", float(pgn::norm_inf(me_pos - tgt_pos)));
			return eStatus::Success;
		}

		//------------------------------------------------------------------------------------------------------
	}
}