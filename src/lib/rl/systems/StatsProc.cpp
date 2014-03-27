#include "StatsProc.h"

#include <ecs/events.h>
#include <rl/event/events.h>
#include <rl/components/components.h>
#include <rl/systems/Teleport.h>
#include <rl/systems/UpdateLayout.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			/*
				evade = 10+ mod(dex)
				att = 1d20 + mod(dex)
				dmg = 1d8 + mod(str)
				hit = attacker.att >= defender.evade
				phys_crit_chance = 5% + mod(dex)%
				phys_crit_dmg = 100% + 10*(mod(str) + mod(dex))%

				gain level:
					random +1 of (str,dex,con)
					add hit die
					recalc maxhealth
					refill current health
			*/

			void cStatsProc::MeleeAttack(ecs::cEntityWithData eatt, ecs::cEntityWithData edef)
			{
				auto att = RollAttack(eatt);
				auto ev = RollEvade(edef);

				if (att > ev)
				{
					// TODO: hit message
					auto dmg = RollDamage(eatt);
					auto& tgt_creature = edef->second.Component<ecs::cmp::cCreature>()->mCreatureData;
					tgt_creature.mCurHealth -= dmg;
					mainapp()->GameLog().Inf(pgn::format("%s hits %s for %d damage", eatt->second.mName.c_str(), edef->second.mName.c_str(), dmg));
					if (tgt_creature.mCurHealth <= 0)
					{
						// TODO: eatt gains exp points
						auto& att_creature = eatt->second.Component<ecs::cmp::cCreature>()->mCreatureData;
						att_creature.mXP += tgt_creature.mMaxHealth * 10;

						// TODO: edef dies
						mainapp()->GameLog().Inf(pgn::format("%s dies!", edef->second.mName.c_str()));
						evt::cCreatureDied::Sig().emit(edef);
						auto& loc = *edef->second.Component<ecs::cmp::cLocation>();
						auto oldloc = loc;
						loc.mLevelId = -1;
						mainecs()->System<ecs::sys::cUpdateLayout>()(edef, &oldloc);
						mainecs()->Destroy(edef);
						// TODO: later on, move the TakeDamage data elsewehere
					}
				}
				else
				{
					mainapp()->GameLog().Inf(pgn::format("%s misses %s", eatt->second.mName.c_str(), edef->second.mName.c_str()));
				}
			}

			int cStatsProc::StatMod(const int stat)
			{
				return (stat >> 1) - 5;
			}

			void cStatsProc::InitCreature(ecs::cEntityWithData ed)
			{
				auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				creature.mCurHealth = creature.mMaxHealth = RollHP(ed);
				creature.mLevel = 1;
				creature.mXP = 0;
				creature.mCurStats = creature.mBaseStats;
				return;
			}

			int cStatsProc::RollHP(ecs::cEntityWithData ed)
			{
				auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				return creature.mHitDie.Roll() + StatMod(creature.mBaseStats[ Enum2Int(rl::eStats::Con)]);
			}

			//-------------------------------------------------------------
			int cStatsProc::RollAttack(ecs::cEntityWithData ed)
			{
				auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				return rl::cDice(1,20).Roll() + StatMod(creature.mBaseStats[Enum2Int(rl::eStats::Dex)]);
			}

			//-------------------------------------------------------------
			int cStatsProc::RollDamage(ecs::cEntityWithData ed)
			{
				auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				return std::max(int(1),int(creature.mDamage.Roll() + StatMod(creature.mBaseStats[Enum2Int(rl::eStats::Str)])));
			}

			//-------------------------------------------------------------
			int cStatsProc::RollEvade(ecs::cEntityWithData ed)
			{
				auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				return rl::cDice(1, 20).Roll() + StatMod(creature.mBaseStats[Enum2Int(rl::eStats::Dex)]);
				return 0;
			}

			//-------------------------------------------------------------
			int cStatsProc::RollCritChance(ecs::cEntityWithData ed)
			{
				//auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				//return rl::cDice(1, 100).Roll() + StatMod(creature.mBaseStats[Enum2Int(rl::eStats::Dex)]);
				return 0;
			}

			//-------------------------------------------------------------
			int cStatsProc::RollCritDmg(ecs::cEntityWithData ed)
			{
				//auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				return 0;
			}

			//-------------------------------------------------------------
			void cStatsProc::AdvanceLevel(ecs::cEntityWithData ed)
			{
				auto& creature = ed->second.Component<cmp::cCreature>()->mCreatureData;
				creature.mCurHealth = creature.mMaxHealth += RollHP(ed);
				creature.mLevel ++;
				creature.mXP = 0;
				creature.mCurStats = creature.mBaseStats;
				return;
			}

			//-------------------------------------------------------------

			
		}
	}
}