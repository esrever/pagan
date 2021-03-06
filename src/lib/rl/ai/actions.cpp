#include "actions.h"
#include "blackboard.h"

#include <ecs/ecs.h>
#include <rl/event/events.h>
#include <rl/components/components.h>
#include <rl/systems/MoveAdj.h>
#include <rl/systems/StatsProc.h>
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
			cAction::Register("FleeTarget", &FleeTarget);
		}

		//------------------------------------------------------------------------------------------------------
		eStatus ApproachTarget(cBlackBoard& bb)
		{
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			auto tgt = *bb.mDictTemp.Get<ecs::cEntityWithData>("target");
			
			pgn::ecs::cmp::cLocation * me_loc = me->second.Component<pgn::ecs::cmp::cLocation>();
			pgn::ecs::cmp::cLocation * tgt_loc = tgt->second.Component<pgn::ecs::cmp::cLocation>();

			const auto& world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
			const auto& lvl = world->mLevelMap.find(me_loc->mLevelId)->second;
			const auto& layout = lvl->second.Component<ecs::cmp::cLevelData>()->mLayout;

			std::function< float(const glm::ivec2&)> dfunc;
			// request difi
			pgn::ecs::cmp::cMapDiFi * tgt_difi = tgt->second.Component<pgn::ecs::cmp::cMapDiFi>();
			if (!tgt_difi)
			{
				// make the distance function
				dfunc = [&](const glm::ivec2& off)->float{
					auto p = me_loc->mPos + off;
					if (layout.Obstacles().InRange(p) && (!layout.Obstacles()(p)))
						return pgn::norm_2(p - tgt_loc->mPos);
					else
						return std::numeric_limits<float>::max();
				};
			}
			else
			{
				dfunc = [&](const glm::ivec2& off)->float{ 
					auto pos = me_loc->mPos + off;
					auto pos_difi = pos - tgt_difi->mValue.CornerWcs();
					if (layout.Obstacles()(pos))
						return std::numeric_limits<float>::max();
					else
					{
						if (tgt_difi->mValue.Data().InRange(pos_difi))
							return tgt_difi->mValue.Data()(pos_difi);
						else
							return pgn::norm_2(pos - tgt_loc->mPos);
					}
				};
			}

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
			pgn::ecs::cmp::cLocation * me_loc = me->second.Component<pgn::ecs::cmp::cLocation>();

			auto world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
			auto lvl = world->mLevelMap[me_loc->mLevelId]->second.Component<ecs::cmp::cLevelData>();
			
			// Find an available adjacent square
			// TODO: move somewhere else!
			auto iters = rl::cShapeCalc< rl::cBoxDistance>::Get(1, 1);
			std::vector<glm::ivec2> avail_pos;
			for (auto it = iters.first; it != iters.second; ++it)
			{
				auto newpos = me_loc->mPos + (*it);
				if (!lvl->mLayout.Bg().Cells().InRange(newpos))
					continue;
				if (!lvl->mLayout.Obstacles()(newpos))
				{
					avail_pos.push_back(*it);
				}
			}

			if (!avail_pos.empty())
				bool ok = mainecs()->System<ecs::sys::cMoveAdj>()(me, avail_pos.at( rand() % avail_pos.size()));
			else
				mainecs()->System<ecs::sys::cMoveAdj>()(me, glm::ivec2(0,0));

			return avail_pos.empty() ? eStatus::Failure : eStatus::Success;
		}

		//------------------------------------------------------------------------------------------------------
		eStatus MeleeAttackTarget(cBlackBoard& bb)
		{
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			auto target = *bb.mDictTemp.Get<ecs::cEntityWithData>("target");	
			mainecs()->System<ecs::sys::cStatsProc>().MeleeAttack(me, target);
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
			// TODO: I assume that the only hostile for the AI is the player
			auto hero = pgn::mainecs()->TagusToEntities("Player");
			const auto& hero_pos = hero->second->second.Component<ecs::cmp::cLocation>()->mPos;
			const auto& hero_vis = hero->second->second.Component<ecs::cmp::cVisibility>()->mVisible;
			pgn::ecs::cmp::cMapDiFi * hero_difi = hero->second->second.Component<pgn::ecs::cmp::cMapDiFi>();
			
			// TODO: I assume that the sensor has SimpleVisibility
			auto los = me->second.Component<ecs::cmp::cSimpleVisibility>()->mLoS;

			// NOTE: Cheating here. If hero sees us, we see him.
			bool seen = hero_vis(me_pos);
			
			// NOTE: Alternatively, if we have a difi, sample the difi map and calc range and compare values!
			// if same, no wall, else wall
			if (hero_difi)
			{
				auto dist = norm_2(hero_pos - me_pos);
				auto pos_difi = me_pos - hero_difi->mValue.CornerWcs();
				if (hero_difi->mValue.Data().InRange(pos_difi))
					seen = (hero_difi->mValue.Data()(pos_difi) < (dist + 1.0f)) && (dist <= float(los));
			}


			if (seen)
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
		eStatus FleeTarget(cBlackBoard& bb)
		{
			// TODO: better fleeing strategy: need longer-term thinking, not just next-square.
			// Planning ahead for N moves requires increasing intelligence.
			// easy: I need to do a search for a difi dist of N, starting at my current monster pos.
			auto me = *bb.mDictPerm.Get<ecs::cEntityWithData>("me");
			auto tgt = *bb.mDictTemp.Get<ecs::cEntityWithData>("target");

			pgn::ecs::cmp::cLocation * me_loc = me->second.Component<pgn::ecs::cmp::cLocation>();
			pgn::ecs::cmp::cLocation * tgt_loc = tgt->second.Component<pgn::ecs::cmp::cLocation>();

			const auto& world = mainecs()->TagusToEntities("World")->second->second.Component<ecs::cmp::cWorldData>();
			const auto& lvl = world->mLevelMap.find(me_loc->mLevelId)->second;
			const auto& layout = lvl->second.Component<ecs::cmp::cLevelData>()->mLayout;

			std::function< float(const glm::ivec2&)> dfunc;
			// request difi
			pgn::ecs::cmp::cMapDiFi * tgt_difi = tgt->second.Component<pgn::ecs::cmp::cMapDiFi>();
			if (!tgt_difi)
			{
				// make the distance function
				dfunc = [&](const glm::ivec2& off)->float{
					auto p = me_loc->mPos + off;
					if (layout.Obstacles().InRange(p) && (!layout.Obstacles()(p)))
						return pgn::norm_2(p - tgt_loc->mPos);
					else
						return std::numeric_limits<float>::max();
				};
			}
			else
			{
				dfunc = [&](const glm::ivec2& off)->float{
					auto pos = me_loc->mPos + off;
					auto pos_difi = pos - tgt_difi->mValue.CornerWcs();
					if (layout.Obstacles()(pos))
						return std::numeric_limits<float>::max();
					else
					{
						if (tgt_difi->mValue.Data().InRange(pos_difi))
							return tgt_difi->mValue.Data()(pos_difi);
						else
							return pgn::norm_2(pos - tgt_loc->mPos);
					}
				};
			}

			// find the closest point to target
			auto iters = rl::cShapeCalc< rl::cBoxDistance>::Get(0, 1);
			auto best_it = iters.first;
			auto best_d = std::numeric_limits<float>::max();
			for (auto it = iters.first; it != iters.second; ++it)
			{
				auto d = dfunc(*it);
				// change sign so we select the furthest distance
				// TODO: this might be crap with movecosts!
				if (d != std::numeric_limits<float>::max())
					d = -d;
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
	}
}