#include <pystring.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>

#include <core/util/color.h>
#include <io/image/utils.h>
#include <dungen/dungen.h>

int main(int argc, char ** argv)
{
	std::srand(0);

	pgn::dungen::cWorkspace ws;
	ws.mConstraints.RoomRectW(glm::ivec2(3, 12))
		.RoomRectH(glm::ivec2(3, 12))
		.DoorToDoorMinDistance(glm::ivec2(2, 2))
		.DoorsPerRoom(glm::ivec2(1, 4))
		.ConnectIslands(true)
		.StoreRoomConnectivity(true)
		.OverrideMaxConns(false)
		.RandomEntryExit(true);

	//ws.Init(59, 53);
	//ws.Init(1024, 1024);
	//ws.Init(299, 93);
	//ws.Init(1024, 1024);
	ws.Init(80, 25);
	//ws.Init(150, 50);
	//ws.Init(10, 20);
	//initscr();ws.Init(COLS-1, LINES-1);
	//timeBeginPeriod(1);
	//auto t0 = timeGetTime();
	ws.generate_dungeon(1000000);
	//auto t1 = timeGetTime();
	//std::cout << "A* runs: " << cAstarMulti::mTotalRuns << std::endl;
	//std::cout << t1 - t0 << " msecs" << std::endl;


	ws.add_feature_entry_exit();

	//auto_explore(ws);
	ws.Print();
	return 0;
}