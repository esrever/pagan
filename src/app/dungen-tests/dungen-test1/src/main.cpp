#include <pystring.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>

#include <core/util/color.h>
#include <io/image/utils.h>
#include <rl/dungen/dungen.h>

int main(int argc, char ** argv)
{
	std::string exepath = argv[0];
	exepath = pystring::replace(exepath, "/", "\\");
	std::vector<std::string> chunks;
	pystring::rsplit(exepath, chunks, "\\", 1);
	exepath = chunks[0] + "\\";

	auto surface = IMG_Load("c:\\users\\babis\\desktop\\rogue.png");
	pgn::cArray2D<bool> mask(surface->w, surface->h);
	auto fmt = surface->format;
	SDL_LockSurface(surface);

	auto pixels = (Uint8 *)surface->pixels;
	assert(surface->pitch == surface->w);
	for (int i = 0; i < surface->h;++i)
		for (int j = 0; j < surface->w;++j)
		{
			const auto& index = pixels[j + i*surface->w];
			const auto& color = fmt->palette->colors[index];
			mask(j,i) = color.r == 0;
		}

	/* Unlock the surface */
	SDL_UnlockSurface(surface);

	std::srand(0);

	pgn::rl::cWorkspace ws;
	ws.mConstraints.RoomRectW(glm::ivec2(3, 12))
		.RoomRectH(glm::ivec2(3, 12))
		.DoorToDoorMinDistance(glm::ivec2(2, 2))
		.DoorsPerRoom(glm::ivec2(1, 4))
		.ConnectIslands(true)
		.StoreRoomConnectivity(true)
		.OverrideMaxConns(true)
		.RandomEntryExit(false);

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
	//ws.generate_dungeon(1000000);
	ws.generate_dungeon_from_mask(INT_MAX, mask);
	//auto t1 = timeGetTime();
	//std::cout << "A* runs: " << cAstarMulti::mTotalRuns << std::endl;
	//std::cout << t1 - t0 << " msecs" << std::endl;


	ws.add_feature_entry_exit();

	//auto_explore(ws);
	//ws.Print();


	
	// Generate a color mapping ( int -> real colours)
	//SDL_Color cCorr, cRoom, cBlocked, cPerim, cConn, cEntryExit;

	// Apply a function from an array to another

	pgn::cArray2D<SDL_Color> imgData(ws.mMapData.Width(), ws.mMapData.Height());
	auto cmapfunc = [&](size_t x, size_t y, const int& elem) { 
		glm::ivec3 col(0, 0, 0);
		if (elem & pgn::rl::eMapData::corridor)
			col += glm::ivec3(255, 255, 0);
		if (elem & pgn::rl::eMapData::room)
			col += glm::ivec3(0, 255, 255);
		if (elem & pgn::rl::eMapData::blocked)
			col -= glm::ivec3(16, 16, 16);
		if (elem & pgn::rl::eMapData::perimeter)
			col += glm::ivec3(64, 64, 64);
		// conn -> green
		if (elem & pgn::rl::eMapData::conn)
			col += glm::ivec3(255, -255, -255);

		if (elem & (pgn::rl::eMapData::entry | pgn::rl::eMapData::exit))
			col = glm::ivec3(0, 0, 255);
		imgData(x, y).r = std::min(std::max(col.x, 0),255);
		imgData(x, y).g = std::min(std::max(col.y, 0), 255);
		imgData(x, y).b = std::min(std::max(col.z, 0), 255);
		imgData(x, y).a = 255;
	};

	ws.mMapData.View().VisitRext(cmapfunc);

	pgn::io::SaveImage(exepath + "dungen-test1.png",imgData);
	return 0;
}