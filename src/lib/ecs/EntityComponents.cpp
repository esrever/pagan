#include "EntityComponents.h"

#include <cassert>

namespace pgn
{
	//-----------------------------------------------------------------
	void cEntityComponents::AddComponent(cComponentBaseSptr zCompo)
	{
		auto idx = zCompo.get()->TypeIndex();
		assert(!mMask.at(idx));
		mMask.at(idx) = 1;
		mComponents.insert(std::pair<unsigned short, cComponentBaseSptr>(idx, zCompo));
	}

	//-----------------------------------------------------------------
	void cEntityComponents::RemoveComponent(unsigned short zTypeId)
	{
		assert(mMask.at(zTypeId));
		mMask.at(zTypeId) = 0;
		mComponents.erase(zTypeId);
	}
}