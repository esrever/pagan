#pragma once

#include <cstdint>
#include <array>

#include <core/serialize/serialize.h>
#include <core/util/enum.h>

#include <rl/utils/dice.h>

namespace pgn
{
	namespace rl
	{
		enum class eStats : int8_t
		{
			Str = 0,
			Dex,
			Con,
			num
		};

		typedef std::array< int, size_t(eStats::num)> cStats;

		struct cCreature
		{
			cStats mBaseStats;
			cStats mCurStats;

			cDice mHitDie;
			cDice mDamage;

			int mMaxHealth;
			int mCurHealth;
			
			int mXP;
			int mLevel;
		};
	}

	DECL_SERIALIZE_INTERFACE(rl::cStats);
	DECL_SERIALIZE_INTERFACE(rl::cCreature);
}