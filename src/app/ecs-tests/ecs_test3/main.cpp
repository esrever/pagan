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
	virtual const std::string ReceiverName() const {return "SystemMgrEx";}
	virtual std::shared_ptr<pgn::cSystemBase> Create(const std::string& zName,
												const std::string& zType,
												const std::string& zDesc,
												const std::vector<std::string>& zQueriesUsed,
												const rapidjson::Value& zInitData)
	{
		std::cout<<"Custom system creation func"<<std::endl;
		return nullptr;
	}
};

class cEntityMgrEx : public pgn::cEntityMgr 
{
public:
	virtual const std::string ReceiverName() const {return "EntityMgrEx";}
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

	//! Init custom types
	ecs.mEntityMgr->AddComponentType<pgn::cExample>();

	//! Read ecs configuration
	ecs.from_json( *pgn::file_to_json("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\ecs.json"));

	ecs.Destroy();
	return 0;
}