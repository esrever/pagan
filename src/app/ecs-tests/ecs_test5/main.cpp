#include <rapidjson/prettywriter.h>	// for stringify JSON
#include <rapidjson/filestream.h>	// wrapper of C stream for prettywriter as output

#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
#include <rl/ActionMgrRL.h>

/*
	Provide json conversions for all components
*/


int main()
{
	//##############################
	//# ECS Init

	auto& ecs = ECS;	

	{
		//! Init log stuff. TODO: Do I need a proper entity? SysLogWindow e.g.
		ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(std::make_shared<pgn::cLogString>(pgn::cLogString()));

		//! Init ecs
		ecs.Init();
		ecs.mSystemMgr = std::shared_ptr<pgn::cSystemMgr>(new pgn::cSystemMgrRL());
		ecs.mEntityMgr = std::shared_ptr<pgn::cEntityMgr>(new pgn::cEntityMgrRL());
		ecs.mActionMgr = std::shared_ptr<pgn::cActionMgr>(new pgn::cActionMgrRL());

		//! register custom types
		ecs.mEntityMgr->RegisterComponentTypes();

		//! register system types
		ecs.mSystemMgr->RegisterSystemTypes();

		//! register action types
		ecs.mActionMgr->RegisterActionTypes();

		//! Read ecs configuration
		//ecs.SetDataPath("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\rl\\data\\");
		ecs.from_json(*pgn::file_to_json(ecs.GetDataPath() + "ecs.json"));

		//###############################
		//# Demo
		// Create 2 entities, a rat and a human
		ecs.mEntityMgr->InstantiateExemplar("rat_common");
		ecs.mEntityMgr->InstantiateExemplar("human_common");

		FILE * fp = fopen("ecs_export.txt","wt");
		rapidjson::StringBuffer strbuf;
		JsonWriter writer(strbuf);
		to_json(ecs, writer);
		const char * jsontext = strbuf.GetString();
		fwrite(jsontext, 1, strlen(jsontext), fp);
		fclose(fp);

		//##############################
		//# Loop

		//int prevposx=posx,prevposy=posy;
		while (ecs.mSystemMgr->UpdateFrame());

	}

	//##############################
	//# ECS End
	ecs.Destroy();

	return 0;
}