#include "ExampleComponent.h"

namespace pgn
{
	//------------------------------------------------------------------------
	template<>
	void to_json<cExample>(const cExample& zObj, rapidjson::Value& zRoot)
	{
	}

	//------------------------------------------------------------------------
	template<>
	void from_json<cExample>(cExample& zObj, const rapidjson::Value& zRoot)
	{
	}
}