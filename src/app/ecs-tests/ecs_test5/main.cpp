#include <curses.h>

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
	//# Curses init
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

	auto v = LINES;
	auto v2 = COLS;

	//##############################
	//# ECS Init

	auto& ecs = ECS;
	//! Init logging
	ecs.Init();

	//! Init managers
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
	ecs.from_json( *pgn::file_to_json(ecs.GetDataPath() + "ecs.json"));

	//###############################
	//# Demo
	// Create 2 entities, a rat and a human
	ecs.mEntityMgr->CloneExemplar("rat_common");
	ecs.mEntityMgr->CloneExemplar("human_common");

	//##############################
	//# Loop
	
	//int prevposx=posx,prevposy=posy;
	while( ecs.mSystemMgr->UpdateFrame() );

	//##############################
	//# Curses init

	endwin();			/* End curses mode		  */

	//##############################
	//# ECS End
	ecs.Destroy();
	return 0;
}