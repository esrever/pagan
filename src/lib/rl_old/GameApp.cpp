#include "GameApp.h"

#include <iostream>

#include <rapidjson/prettywriter.h>	// for stringify JSON
#include <rapidjson/filestream.h>	// wrapper of C stream for prettywriter as output

#include <core/util/json_conversions.h>
#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
#include <rl/ActionMgrRL.h>
#include <rl/GameApp.h>

#include <rl/components/MapSprite.h>
#include <rl/components/Level.h>

namespace pgn
{
	void cGameApp::Init()
	{
		//! Init ecs
		auto& ecs = ECS;
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

		// Get data path
		mDataPath = __FILE__;
		mDataPath.erase(mDataPath.end() - 11, mDataPath.end());
		mDataPath.append("data/");

		// Load config file
		const std::string cfg = mDataPath + "game.json";
		auto doc = file_to_json(cfg);
		if (doc)
			from_json(*this, *doc);

		//###############################
		//# Demo
		// Create 2 entities, a rat and a human
		
		ecs.mEntityMgr->InstantiateExemplar("rat_common");
		ecs.mEntityMgr->InstantiateExemplar("human_common");
	

		// Now do the level loading stuff
		auto ed = ecs.mEntityMgr->TaggedEntity("CurrentLevel");
		pgn::cActionLevelCreate::RunEvent(ed);
		pgn::cActionLevelLoad::RunEvent(ed);

		FILE * fp = fopen("ecs_export.txt", "wt");
		rapidjson::StringBuffer strbuf;
		JsonWriter writer(strbuf);
		to_json(ecs, writer);
		const char * jsontext = strbuf.GetString();
		fwrite(jsontext, 1, strlen(jsontext), fp);
		fclose(fp);
	}

	void cGameApp::Destroy()
	{
		ECS.Destroy();
	}

	void cGameApp::UpdateFrame()
	{
		ECS.mSystemMgr->UpdateFrame();
	}

	template<>
	bool from_json< cGameApp >(cGameApp & zData, const rapidjson::Value& zRoot)
	{
		from_json(zData.mIO, zRoot["IO"]);
		from_json(zData.mResources, zRoot["Resources"]);
		auto& ecs = ECS;
		ecs.SetDataPath(zData.GetDataPath());
		ecs.from_json(zRoot["ecs"]);
		return true;
	}

	template<>
	void to_json< cGameApp >(const cGameApp & zData, JsonWriter& zRoot)
	{

	}
}