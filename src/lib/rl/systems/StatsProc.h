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

					void MeleeAttack(ecs::cEntityWithData att, ecs::cEntityWithData def);
					int StatMod(const int stat);
					void InitCreature(ecs::cEntityWithData);
					int RollHP(ecs::cEntityWithData);
					int RollAttack(ecs::cEntityWithData);
					int RollDamage(ecs::cEntityWithData);
					int RollEvade(ecs::cEntityWithData);
					int RollCritChance(ecs::cEntityWithData);
					int RollCritDmg(ecs::cEntityWithData);
					void AdvanceLevel(ecs::cEntityWithData);
			};
		}
	}
}