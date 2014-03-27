#pragma once

#include <cstdint>
#include <core/util/string.h>
#include <core/serialize/serialize.h>

namespace pgn
{
	namespace rl
	{
		struct cDice
		{
			cDice(uint32_t times=1, uint32_t sides=20):mTimes(times),mSides(sides){}
			uint32_t mTimes;
			uint32_t mSides;

			uint32_t Roll() const;
		};
	}

	DECL_SERIALIZE_INTERFACE(rl::cDice);
}

namespace std
{
	inline const string to_string(const pgn::rl::cDice& dice)
	{
		std::ostringstream oss;
		oss << dice.mTimes << 'd' << dice.mSides;
		return oss.str();
	}

	inline istream& operator>> (istream &in, pgn::rl::cDice& dice)
	{
		char c;
		in >> dice.mTimes >> c >> dice.mSides;
		return in;
	}
}