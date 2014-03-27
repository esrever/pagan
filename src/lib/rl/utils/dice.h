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
			uint32_t mTimes;
			uint32_t mSides;

			uint32_t Roll();
		};
	}
}

namespace std
{
	template < class T>
	inline const string to_string(const pgn::rl::cDice& dice)
	{
		std::ostringstream oss;
		oss << dice.mTimes << 'd' << dice.mSides;
		return oss.str();
	}

	template<class T>
	inline istream& operator>> (istream &in, pgn::rl::cDice& dice)
	{
		char c;
		in >> dice.mTimes >> c >> dice.mSides;
	}
}