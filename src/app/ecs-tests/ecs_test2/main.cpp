#include <ecs/EntityMgr.h>
#include <ecs/Component.h>

#include <core/util/json_conversions.h>

#include <rapidjson/reader.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filestream.h>
#include <rapidjson/stringbuffer.h>

/*
	Provide json conversions for all components
*/

namespace pgn
{
	template <>
	void to_json<pgn::cComponent<int>>(const pgn::cComponent<int>& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("an_int",zObj.mData, ECS.JsonAllocator() );
	}

	template <>
	void to_json<pgn::cComponent<double>>(const pgn::cComponent<double>& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("a_double",zObj.mData, ECS.JsonAllocator());
	}

	template <>
	void to_json<pgn::cComponent<float>>(const pgn::cComponent<float>& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("a_float",zObj.mData, ECS.JsonAllocator());
	}
}

int main()
{
	auto ecs = ECS;
	pgn::cComponent<int> iComp;  iComp.mData = 2;
	pgn::cComponent<double> dComp; dComp.mData = 3.0;
	pgn::cComponent<float> fComp; fComp.mData= 4.0f;

	{
		auto entity = ecs.Create();
		ecs.AddComponent(entity, iComp);
		ecs.AddComponent(entity, dComp);
		ecs.AddComponent(entity, fComp);
		ecs.RemoveComponent(entity, iComp);
		ecs.RemoveComponent(entity, fComp);
		ecs.RemoveComponent(entity, dComp);
	}
	return 0;
}