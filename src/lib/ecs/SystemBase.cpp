#include "SystemBase.h"

#include <boost/format.hpp>

#include "ecs.h"
#include "Event.h"

namespace pgn
{
	//---------------------------------------------------------------
	cSystemBase::cSystemBase()
		:mActive(false)
	{}

	//---------------------------------------------------------------
	cSystemBase::~cSystemBase()
	{
		SetActive(false);
	}
	
	//---------------------------------------------------------------
	void cSystemBase::SetActive(bool zActive)
	{
		const cSystemBase * const ptr = this;
		cSystemActiveEvent::mSig.emit( this, zActive);
	}

	//---------------------------------------------------------------
	bool cSystemBase::from_json(const rapidjson::Value& zRoot)
	{
		// Read Desc
		mDesc = zRoot.HasMember("Desc") ? zRoot["Desc"].GetString() : "";
		mName = zRoot.HasMember("Name") ? zRoot["Name"].GetString() : "";
		return true;
	}

	//---------------------------------------------------------------
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, JsonWriter& zRoot) 
	{
		zSys.to_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	bool from_json<cSystemBase>(cSystemBase& zSys, const rapidjson::Value& zRoot) 
	{
		return zSys.from_json(zRoot);
	}

	void cSystemBase::to_json(JsonWriter& zRoot) const
	{
		zRoot.StartObject();
		JsonWriter_AddMember("Name", mName,zRoot);
		JsonWriter_AddMember("Desc", mDesc, zRoot);
		zRoot.EndObject();
	}
}