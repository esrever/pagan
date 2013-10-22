#include "ExampleSystem.h"

namespace pgn
{
	//------------------------------------------------------------------------
	void cExampleSystem::to_json(rapidjson::Value& zRoot) const
	{
		zRoot.SetObject();
		//zRoot.AddMember("an_int",zObj.a );
		//zRoot.AddMember("a_double",zObj.b );
		//zRoot.AddMember("a_float",zObj.c );		
	}

	//------------------------------------------------------------------------
	void cExampleSystem::from_json(const rapidjson::Value& zRoot)
	{
		cSystemBase::from_json(zRoot);
		//zObj.a = zRoot["an_int"].GetInt();
		//zObj.b = zRoot["a_double"].GetDouble();
		//zObj.c = float(zRoot["a_float"].GetDouble());
	}

	//------------------------------------------------------------------------
	void cExampleSystem::Process()
	{
	}
}