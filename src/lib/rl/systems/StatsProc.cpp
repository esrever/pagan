#include "StatsProc.h"

#include <rl/event/events.h>
#include <rl/components/components.h>

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

			//-------------------------------------------------------------
			bool cStatsProc::operator()()
			{
				return true;
			}

			//-------------------------------------------------------------
			int cStatsProc::CalcNewLevelHP(ecs::cEntityWithData)
			{
				// roll HD and add StatModifier( stats[con] )
				return 1;
			}

			//-------------------------------------------------------------
			float cStatsProc::ExpGainMult(ecs::cEntityWithData)
			{
				return 1.0f;
			}

			//-------------------------------------------------------------
			int cStatsProc::PhysDmg(ecs::cEntityWithData ed)
			{
				// roll 1d8 and add StatModifier( stats[str])
				auto stats = ed->second.Component<cmp::cStats>();
				return 0;
			}

			//-------------------------------------------------------------
			float cStatsProc::CritDmgMult(ecs::cEntityWithData ed)
			{
				auto stats = ed->second.Component<cmp::cStats>();
				stats->mDexterity - 10;
				return 0.0f;
			}

			//-------------------------------------------------------------
			float cStatsProc::CritChance(ecs::cEntityWithData)
			{
				// roll 1d20 and add StatModifier( stats[dex])
				return 0.0f;
			}

			//-------------------------------------------------------------
			float cStatsProc::HitChance(ecs::cEntityWithData)
			{
				// everyone starts with 10 evade value
				// 
				// roll 1d20 and add StatModifier( stats[dex])
				return 0.0f;
			}

			
		}
	}
}