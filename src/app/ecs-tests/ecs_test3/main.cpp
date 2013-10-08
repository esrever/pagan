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

class cSystemMgrEx : public pgn::cSystemMgr
{
public:
	virtual void RegisterSystemTypes()
	{
		AddSystemType<pgn::cExampleSystem>();
	}
	virtual const std::string ReceiverName() const {return "SystemMgrEx";}

};

class cEntityMgrEx : public pgn::cEntityMgr 
{
public:
	virtual const std::string ReceiverName() const {return "EntityMgrEx";}
	virtual void RegisterComponentTypes()
	{
		AddComponentType<pgn::cExample>();
	}
};

namespace pgn
{
	template <>
	void to_json<int>(const int& zObj, rapidjson::Value& zRoot)
	{
		zRoot.SetObject();
		zRoot.AddMember("an_int",zObj );
	}
	template <> void from_json<int>(int& obj, const rapidjson::Value& root){ obj = root["an_int"].GetInt();}
}


int main()
{
	auto& ecs = ECS;
	//! Init logging
	ecs.Init();

	//! Init managers
	ecs.mSystemMgr = std::shared_ptr<pgn::cSystemMgr>(new cSystemMgrEx());
	ecs.mEntityMgr = std::shared_ptr<pgn::cEntityMgr>(new cEntityMgrEx());

	//! register custom types
	ecs.mEntityMgr->RegisterComponentTypes();

	//! register system types
	ecs.mSystemMgr->RegisterSystemTypes();

	//! Read ecs configuration
	ecs.from_json( *pgn::file_to_json("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\ecs.json"));

	ecs.Destroy();
	return 0;
}