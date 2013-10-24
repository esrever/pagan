#include "ComponentBase.h"

namespace pgn
{
	//---------------------------------------------------------------
	template<>
	void to_json<cComponentBase>(const cComponentBase& zCompo, rapidjson::Value& zRoot) 
	{
		zCompo.to_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	bool from_json<cComponentBase>(cComponentBase& zCompo, const rapidjson::Value& zRoot) 
	{
		return zCompo.from_json(zRoot);
	}

	//---------------------------------------------------------------
	template<>
	std::string to_string<cComponentBase>(const cComponentBase& zCompo) 
	{
		rapidjson::Value root;
		zCompo.to_json(root);
		return to_string(root);
	}
}