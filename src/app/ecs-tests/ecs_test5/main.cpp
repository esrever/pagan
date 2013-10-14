#include <curses.h>

#include <ecs/ecs.h>
#include <rl/EntityMgrRL.h>
#include <rl/SystemMgrRL.h>
#include <rl/EntityMgrRL.h>

/*
	Provide json conversions for all components
*/


int main()
{
	//##############################
	//# ECS Init

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

	//##############################
	//# Curses init

	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

	//##############################
	//# Loop

	
	int prevposx=posx,prevposy=posy;
	while( true )
	{
		//! Convert actions into moves 
		//cActionMappingSystem.Process();
		// chtype ch = getch();
		//if(ch == 27) break;

		cGameTurnSystem.Process();
		cAsciiRenderSystem.Process();
		//refresh();
	}

	//##############################
	//# Curses init

	endwin();			/* End curses mode		  */

	//##############################
	//# ECS End
	ecs.Destroy();
	return 0;
}