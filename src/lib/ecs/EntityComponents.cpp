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

	//---------------------------------------------------------------------------
	template<>
	void to_json<cEntityComponents>(const cEntityComponents& zObj, JsonWriter& writer)
	{
		writer.StartObject();
		JsonWriter_AddMember("Mask", zObj.Mask().to_string(), writer);
		JsonWriter_AddMember("Components", zObj.Components(), writer);
		writer.EndObject();
	}
}