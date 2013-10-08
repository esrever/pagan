#include "ExampleSystem.h"

namespace pgn
{
	//------------------------------------------------------------------------
	template<>
	void to_json<cExampleSystem>(const cExampleSystem& zObj, rapidjson::Value& zRoot)
	{
	}

	//------------------------------------------------------------------------
	template<>
	void from_json<cExampleSystem>(cExampleSystem& zObj, const rapidjson::Value& zRoot)
	{
	}

	//------------------------------------------------------------------------
	void cExampleSystem::Process()
	{
	}

	//------------------------------------------------------------------------
	void cExampleSystem::ProcessSingle(const cEntity& zE)
	{
	}
}