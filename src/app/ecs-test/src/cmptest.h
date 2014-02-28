#pragma once

#include <ecs/ecs-config.h>

namespace pgn
{
	struct cCmpTest
	{
		cCmpTest(const int& arg0 = int(5)) :mData(arg0){}

		bool SerializeIn(const node_type&);
		void SerializeOut(node_type&, const std::string&) const;
			
		int mData;
	};
}