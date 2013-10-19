#include "EntityComponents.h"

#include "ecs.h"
#include <cassert>

namespace pgn
{
	cEntityComponents::cEntityComponents()
	{
		mComponents.resize(  ECS.mEntityMgr->GetComponentTypeIndexAll().size());
	}
	//-----------------------------------------------------------------
	void cEntityComponents::AddComponent(cComponentBaseSptr zCompo)
	{
		auto idx = zCompo.get()->TypeIndex();
		assert(!mMask.at(idx));
		mMask.at(idx) = 1;
		mComponents.at(idx) = zCompo;
	}

	//-----------------------------------------------------------------
	void cEntityComponents::RemoveComponent(unsigned short zTypeId)
	{
		assert(mMask.at(zTypeId));
		mMask.at(zTypeId) = 0;
		mComponents.at(zTypeId).reset();
	}
}