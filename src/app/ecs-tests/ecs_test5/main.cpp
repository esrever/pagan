#include <curses.h>

#include <rapidjson/prettywriter.h>	// for stringify JSON
#include <rapidjson/filestream.h>	// wrapper of C stream for prettywriter as output

#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
#include <rl/ActionMgrRL.h>
#include <rl/util/LogCurses.h>

#include <rl/components/AsciiWindow.h>

#include <core/util/textmod.h>
#include <rl/curses/textmod.h>
#include <rl/curses/fontpreset.h>

/*
	Provide json conversions for all components
*/


int main()
{
	//##############################
	//# Curses init
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
	start_color();

	//##############################
	//# ECS Init

	auto& ecs = ECS;	

	{
		//! Init log stuff. TODO: Do I need a proper entity? SysLogWindow e.g.
		/*
		pgn::cAsciiWindow awin;
		awin.mStart = glm::ivec2(0, 0);
		awin.mSize = glm::uvec2(COLS, LINES);
		awin.Init();
		pgn::cLogCurses log;
		log.SetWindow(std::make_shared<pgn::cAsciiWindow>(awin));
		ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(std::make_shared<pgn::cLogCurses>(log));
		*/
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

	//##############################
	//# Curses end
	endwin();			/* End curses mode		  */
	return 0;
}