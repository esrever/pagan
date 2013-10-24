#include "ExampleComponent.h"

namespace pgn
{
	//------------------------------------------------------------------------
	template<>
	void to_json<cExample>(const cExample& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("an_int",zObj.a );
		zRoot.AddMember("a_double",zObj.b );
		zRoot.AddMember("a_float",zObj.c );
	}

	//------------------------------------------------------------------------
	template<>
	bool from_json<cExample>(cExample& zObj, const rapidjson::Value& zRoot)
	{
		zObj.a = zRoot["an_int"].GetInt();
		zObj.b = zRoot["a_double"].GetDouble();
		zObj.c = float(zRoot["a_float"].GetDouble());
		return true;
	}
}