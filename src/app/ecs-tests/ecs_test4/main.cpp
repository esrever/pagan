#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
//#include <rl/EntityMgrRL.h>

/*
	Provide json conversions for all components
*/


int main()
{
	auto& ecs = ECS;
	//! Init logging
	ecs.Init();

	//! Init managers
	ecs.mSystemMgr = std::shared_ptr<pgn::cSystemMgr>(new pgn::cSystemMgrRL());
	ecs.mEntityMgr = std::shared_ptr<pgn::cEntityMgr>(new pgn::cEntityMgrRL());

	//! register custom types
	ecs.mEntityMgr->RegisterComponentTypes();

	//! register system types
	ecs.mSystemMgr->RegisterSystemTypes();

	//! Read ecs configuration
	ecs.from_json( *pgn::file_to_json("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\ecs.json"));

	ecs.Destroy();
	return 0;
}