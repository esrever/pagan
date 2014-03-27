#include "dice.h"
#include <rl/app/rlapp.h>

namespace pgn
{
	namespace rl
	{
		uint32_t cDice::Roll()
		{
			std::uniform_int_distribution<> dis(1, mSides);
			uint32_t res = 0;
			auto& rande = pgn::mainapp()->Rand();
			for (uint32_t i = 0; i < mTimes; ++i)
				res += dis(rande);
			return res;
		}
	}
}