#pragma once

#include <core/event/Event.h>
#include <ecs/system.h>
#include <ecs/ecs.h>

#include <rl/components/components.h>

namespace pgn
{
	namespace ecs
	{
		namespace sys
		{
			class cStatsProc : public cBase
			{
				public:
					bool operator()();
					int CalcNewLevelHP(ecs::cEntityWithData);
					float ExpGainMult(ecs::cEntityWithData);
					int PhysDmg(ecs::cEntityWithData);
					float CritDmgMult(ecs::cEntityWithData);
					float CritChance(ecs::cEntityWithData);
					float HitChance(ecs::cEntityWithData);
			};
		}
	}
}