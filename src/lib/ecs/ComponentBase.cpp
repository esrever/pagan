#include "ComponentBase.h"

namespace pgn
{
	//---------------------------------------------------------------
	template<>
	void to_json<cComponentBase>(const cComponentBase& zCompo, JsonWriter& zRoot) 
	{
		zCompo.to_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	bool from_json<cComponentBase>(cComponentBase& zCompo, const rapidjson::Value& zRoot) 
	{
		return zCompo.from_json(zRoot);
	}
}