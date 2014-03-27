#include "creature.h"

namespace pgn
{
	namespace rl
	{
		
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const rl::cStats & value)
	{
		std::ostringstream oss;
		size_t i = 0;
		while (i < value.size())
			oss << value.at(i++)<<',';
		SerializeOut(node, oss.str());
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, rl::cStats & value)
	{
		size_t ret = 0;
		std::string s;
		ret += SerializeIn(node, "value",s);
		if (ret)
		{
			std::istringstream iss(s);
			size_t i = 0;
			char c;
			while (i < value.size())
				iss >> value.at(i++) >> c;
		}
		return ret;
	}

	//---------------------------------------------------------------------------------------------------
	void SerializeOut(node_type& node, const rl::cCreature & value)
	{
		SerializeOut(node, "Stats", value.mBaseStats);
		SerializeOut(node, "HitDie", value.mHitDie);
		SerializeOut(node, "Damage", value.mDamage);
	}

	//---------------------------------------------------------------------------------------------------
	size_t SerializeIn(const node_type& node, rl::cCreature & value)
	{
		size_t ret = 0;
		ret += SerializeIn(node, "Stats", value.mBaseStats);
		value.mCurStats = value.mBaseStats;
		ret += SerializeIn(node, "HitDie", value.mHitDie);
		ret += SerializeIn(node, "Damage", value.mDamage);
		return ret;
	}
}