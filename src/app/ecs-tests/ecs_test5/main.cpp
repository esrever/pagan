#include <curses.h>

#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
#include <rl/ActionMgrRL.h>
#include <rl/util/LogCurses.h>

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

	//##############################
	//# ECS Init

	auto& ecs = ECS;
	

	{
		//! Init log stuff. TODO: Do I need a proper entity? SysLogWindow e.g.
		pgn::cAsciiWindow awin;
		awin.mStart = glm::ivec2(0, 0);
		awin.mSize = glm::uvec2(COLS, LINES);
		awin.Init();
		pgn::cLogCurses log;
		log.SetWindow(std::make_shared<pgn::cAsciiWindow>(awin));
		ecs.mLog = std::dynamic_pointer_cast<pgn::cLogBase>(std::make_shared<pgn::cLogCurses>(log));

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