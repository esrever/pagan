#include "SystemBase.h"

#include <boost/format.hpp>

#include "ecs.h"
#include "Event.h"

namespace pgn
{
	//---------------------------------------------------------------
	template<>
	bool from_json(cSystemBase& zSys, const rapidjson::Value& zRoot)
	{
		// Read Desc
		zSys.mDesc = zRoot.HasMember("Desc") ? zRoot["Desc"].GetString() : "";
		zSys.mName = zRoot.HasMember("Name") ? zRoot["Name"].GetString() : "";
		return true;
	}

	//---------------------------------------------------------------
	template<>
	void to_json<cSystemBase>(const cSystemBase& zSys, JsonWriter& zRoot) 
	{
		zRoot.StartObject();
		JsonWriter_AddMember("Name", zSys.mName, zRoot);
		JsonWriter_AddMember("Desc", zSys.mDesc, zRoot);
		zRoot.EndObject();
	}
}