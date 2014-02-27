#include "ExampleComponent.h"

namespace pgn
{
	//------------------------------------------------------------------------
	template<>
	void to_json<cExample>(const cExample& zObj, JsonWriter& zRoot)
	{
		zRoot.StartObject();
		zRoot.String("an_int");
		zRoot.Uint(zObj.a);
		zRoot.String("a_double");
		zRoot.Double(zObj.b);
		zRoot.String("a_float");
		zRoot.Double(zObj.c);
		zRoot.EndObject();
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