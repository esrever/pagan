#include "StatsProc.h"

#include <rl/event/events.h>
#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			//-------------------------------------------------------------
			bool cStatsProc::operator()()
			{
				return true;
			}

			//-------------------------------------------------------------
			int cStatsProc::CalcNewLevelHP(ecs::cEntityWithData)
			{
				return 1;
			}

			//-------------------------------------------------------------
			float cStatsProc::ExpGainMult(ecs::cEntityWithData)
			{
				return 1.0f;
			}

			//-------------------------------------------------------------
			int cStatsProc::PhysDmg(ecs::cEntityWithData)
			{
				return 0;
			}

			//-------------------------------------------------------------
			float cStatsProc::CritDmgMult(ecs::cEntityWithData)
			{
				return 0.0f;
			}

			//-------------------------------------------------------------
			float cStatsProc::CritChance(ecs::cEntityWithData)
			{
				return 0.0f;
			}

			//-------------------------------------------------------------
			float cStatsProc::HitChance(ecs::cEntityWithData)
			{
				return 0.0f;
			}

			
		}
	}
}