#include <ecs/ecs.h>
#include <ecs/Component.h>

#include <core/util/json_conversions.h>

#include <rapidjson/reader.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filestream.h>
#include <rapidjson/stringbuffer.h>

#include <ecs/ExampleComponent.h>
#include <ecs/ExampleSystem.h>

/*
	Provide json conversions for all components
*/

namespace pgn
{
	template <>
	void to_json<int>(const int& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("an_int",zObj );
	}

	template <>
	void to_json<double>(const double& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("a_double",zObj);
	}

	template <>
	void to_json<float>(const float& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("a_float",zObj);
	}

	template <> void from_json<int>(int& obj, const rapidjson::Value& root){ obj = root["an_int"].GetInt();}
	template <> void from_json<double>(double& obj, const rapidjson::Value& root){  obj = root["a_double"].GetDouble();}
	template <> void from_json<float>(float& obj, const rapidjson::Value& root){  obj = float(root["a_float"].GetDouble());}
}

class cBase{};
class cDer : public cBase{};

template<class T>
void print_ref(const T& o) {std::cout<<"moef-ref"<<std::endl;}
template<>
void print_ref<cBase>(const cBase& o) {std::cout<<"Base-ref"<<std::endl;}
template<>
void print_ref<cDer>(const cDer& o) {std::cout<<"Der-ref"<<std::endl;}

template<class T>
void print_ptr(const T * o) {std::cout<<"moef-ptr"<<std::endl;}
template<>
void print_ptr<cBase>(const cBase * o) {std::cout<<"Base-ptr"<<std::endl;}
template<>
void print_ptr<cDer>(const cDer * o) {std::cout<<"Der-ptr"<<std::endl;}

int main()
{
	/*
	cBase b;
	cDer d;
	cDer * pd= new cDer();
	cBase * pb= new cBase();

	print_ref(b);
	print_ref(d);
	print_ptr(&b);
	print_ptr(&d);
	
	print_ref(*pb);
	print_ref(*pd);
	print_ptr(pb);
	print_ptr(pd);
	*/

	auto& ecs = ECS;
	ecs.Init();
	ecs.from_json( *pgn::file_to_json("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\ecs.json"));

	{
		pgn::cComponent<pgn::cExample> exc;
		exc.mData.a = 1;
		exc.mData.b = 2;
		exc.mData.c = 3.0f;

		rapidjson::Value val;
		pgn::to_json(exc, val);
		const std::string s = pgn::to_string(val);
		std::cout<<s<<std::endl;
		pgn::cComponent<pgn::cExample> exc2;
		pgn::from_json(exc2,val);
		assert(memcmp(&exc,&exc2,sizeof(exc)) == 0);
	}

	std::cout<< pgn::to_string<rapidjson::Value>(*pgn::file_to_json("C:\\Users\\babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\actors.json"));

	{
		pgn::cComponent<int> iComp;  iComp.mData = 2;
		pgn::cComponent<double> dComp; dComp.mData = 3.0;
		pgn::cComponent<float> fComp; fComp.mData= 4.0f;

		pgn::component_mask_type mask_df, mask_id;
		mask_df.set( EMGR->GetComponentTypeIndex("double")).set(EMGR->GetComponentTypeIndex("float")); // query for doubles and floats
		mask_id.set(EMGR->GetComponentTypeIndex("int")).set(EMGR->GetComponentTypeIndex("double")); // query for doubles and floats
		pgn::cComponentQuery q_df(mask_df);
		pgn::cComponentQuery q_id(mask_id);

		auto entity = ecs.mEntityMgr.Create();
		ecs.mEntityMgr.AddComponent(entity, iComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 0);
		ecs.mEntityMgr.AddComponent(entity, dComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 1);
		ecs.mEntityMgr.AddComponent(entity, fComp);
		assert(q_df.Get().size() == 1);
		assert(q_id.Get().size() == 1);
		ecs.mEntityMgr.RemoveComponent(entity, iComp);
		assert(q_df.Get().size() == 1);
		assert(q_id.Get().size() == 0);
		ecs.mEntityMgr.RemoveComponent(entity, fComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 0);
		ecs.mEntityMgr.RemoveComponent(entity, dComp);
		assert(q_df.Get().size() == 0);
		assert(q_id.Get().size() == 0);
	}
	
	{
		pgn::component_mask_type mask_d;
		mask_d.set(EMGR->GetComponentTypeIndex("double"));
		pgn::cComponentQuery q_d(mask_d);
		assert(q_d.Get().size() == 0);
		pgn::cComponent<double> dComp; dComp.mData = 3.0;
		auto entity = ecs.mEntityMgr.Create();
		ecs.mEntityMgr.AddComponent(entity, dComp);
		assert(q_d.Get().size() == 1);
		// ecs.AddComponent(entity, dComp); // would assert!
		ecs.mEntityMgr.Destroy(entity);
		assert(q_d.Get().size() == 0);
	}

	{
		pgn::component_mask_type mask_d;
		mask_d.set(EMGR->GetComponentTypeIndex("double"));
		pgn::cComponentQuery q_d(mask_d);
		assert(q_d.Get().size() == 0);
		{
			pgn::cComponent<double> dComp; dComp.mData = 3.0;
			{
				auto entity = ecs.mEntityMgr.Create();
				ecs.mEntityMgr.AddComponent(entity, dComp);
			}
			//assert(q_d.Get().size() == 0); // entity out of scope -> it's a copy
		}
		//assert(q_d.Get().size() == 0); // component out of scope -> it's a copy
	}

	{
		assert(ecs.mEntityMgr.GetComponentTypeIndex("int") == 1);
		assert(ecs.mEntityMgr.GetComponentTypeIndex("float") == 3);
		assert(ecs.mEntityMgr.GetComponentTypeIndex("double") == 2);
	}
	return 0;
}