#pragma once

#include "Component.h"

namespace pgn
{
	struct cExample
	{
		int a;
		double b;
		float c;
	};

	//------------------------------------------------------------------------
	template<>
	void to_json<cExample>(const cExample& zObj, rapidjson::Value& zRoot);

	//------------------------------------------------------------------------
	template<>
	void from_json<cExample>(cExample& zObj, const rapidjson::Value& zRoot);
}
