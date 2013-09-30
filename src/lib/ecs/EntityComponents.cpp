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
		mComponents.insert(zCompo);
	}

	//-----------------------------------------------------------------
	void cEntityComponents::RemoveComponent(cComponentBaseWptr zCompo)
	{
		auto sptr =zCompo.lock(); 
		auto idx = sptr.get()->TypeIndex();
		assert(mMask.at(idx));
		mMask.at(idx) = 0;
		mComponents.erase(sptr);
	}
}