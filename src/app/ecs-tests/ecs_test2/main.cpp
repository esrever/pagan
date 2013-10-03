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
		zRoot.AddMember("an_int",zObj.mData );
	}

	template <>
	void to_json<pgn::cComponent<double>>(const pgn::cComponent<double>& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("a_double",zObj.mData);
	}

	template <>
	void to_json<pgn::cComponent<float>>(const pgn::cComponent<float>& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("a_float",zObj.mData);
	}
}

int main()
{
	auto& ecs = ECS;
	pgn::cComponent<int> iComp;  iComp.mData = 2;
	pgn::cComponent<double> dComp; dComp.mData = 3.0;
	pgn::cComponent<float> fComp; fComp.mData= 4.0f;

	std::cout<< pgn::to_string<rapidjson::Value>(*pgn::file_to_json("C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\actors.json"));

	pgn::component_mask_type mask_df, mask_id;
	mask_df.set(1).set(2); // query for doubles and floats
	mask_id.set(0).set(1); // query for doubles and floats
	pgn::cComponentQuery q_df(mask_df);
	pgn::cComponentQuery q_id(mask_id);

	{
		auto entity = ecs.Create();
		ecs.AddComponent(entity, iComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 0);
		ecs.AddComponent(entity, dComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 1);
		ecs.AddComponent(entity, fComp);
		assert(q_df.Get().size() == 1);
		assert(q_id.Get().size() == 1);
		ecs.RemoveComponent(entity, iComp);
		assert(q_df.Get().size() == 1);
		assert(q_id.Get().size() == 0);
		ecs.RemoveComponent(entity, fComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 0);
		ecs.RemoveComponent(entity, dComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 0);

		// TODO: keep entity alive without components, or kill it? keep it alive and have a query AND system for destroying empty entities?
		// How to say: I want entities with xyz components SET and abc compoenents NOT set? useful, or make special query?
		/*
		ecs.AddComponent(entity, iComp)
		ecs.AddComponent(entity, dComp);
		assert(q_id.Get().size() == 1);
		ecs.Destroy(entity);
		assert(q_id.Get().size() == 0);
		*/
	}
	
	{

	}
	return 0;
}