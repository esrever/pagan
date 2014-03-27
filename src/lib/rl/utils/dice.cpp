#include "dice.h"
#include <rl/app/rlapp.h>

namespace pgn
{
	namespace rl
	{
		uint32_t cDice::Roll() const
		{
			std::uniform_int_distribution<> dis(1, mSides);
			uint32_t res = 0;
			auto& rande = pgn::mainapp()->Rand();
			for (uint32_t i = 0; i < mTimes; ++i)
				res += dis(rande);
			return res;
		}
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const rl::cDice & value)
	{
		SerializeOut(node, std::to_string(value));
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, rl::cDice & value)
	{
		size_t ret = 0;
		std::string s;
		ret += SerializeIn(node, "value", s);
		if (ret)
			value = std::from_string<rl::cDice>(s);
		return ret;
	}
}