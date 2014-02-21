#include <pystring.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>

#include <core/util/color.h>
#include <io/image/utils.h>

#include <core/container/Array2D.h>
#include <ai/fov/FovLookup.h>
#include <ai/fov/Fov0.h>
#include <ai/fov/Fov1.h>
#include <dungen/dungen.h>

struct cTestCase
{
	std::string name;
	pgn::cArray2D<int> dmap;
	glm::ivec2 pos;
};

std::vector<cTestCase> fovtests;

void GenerateWiggly()
{
	// WIGGLY
	pgn::cArray2D<int> testmap;
	testmap.Resize(10, 4);
	testmap.Fill(pgn::dungen::eMapData::perimeter);
	testmap(1, 1) = pgn::dungen::eMapData::room;
	testmap(2, 1) = pgn::dungen::eMapData::room;
	testmap(2, 2) = pgn::dungen::eMapData::room;
	testmap(3, 2) = pgn::dungen::eMapData::room;
	testmap(4, 2) = pgn::dungen::eMapData::room;
	testmap(4, 1) = pgn::dungen::eMapData::room;
	testmap(5, 1) = pgn::dungen::eMapData::room;
	testmap(6, 1) = pgn::dungen::eMapData::room;
	testmap(6, 2) = pgn::dungen::eMapData::room;
	testmap(7, 2) = pgn::dungen::eMapData::room;
	testmap(8, 2) = pgn::dungen::eMapData::room;
	testmap(8, 1) = pgn::dungen::eMapData::room;
	glm::ivec2 start(8, 2);;
	
	cTestCase test;
	test.name = "Wiggly";
	test.dmap = testmap;
	test.pos = start;
	fovtests.push_back(test);
}

void GenerateDiag()
{
	// DIAG
	pgn::cArray2D<int> testmap;
	testmap.Resize(80, 80);
	testmap.Fill(pgn::dungen::eMapData::room);
	for (size_t i = 0; i < 80; ++i)
		testmap(i, i) = pgn::dungen::eMapData::perimeter;
	glm::ivec2 start(21, 20);

	cTestCase test;
	test.name = "Diag-0";
	test.dmap = testmap;
	test.pos = start;
	fovtests.push_back(test);
}

void GeneratePillar()
{
	// PILLAR
	pgn::cArray2D<int> testmap;
	testmap.Resize(40, 40);
	testmap.Fill(pgn::dungen::eMapData::room);
	testmap(20, 20) = pgn::dungen::eMapData::perimeter;
	glm::ivec2 start(20, 18);

	cTestCase test;
	test.name = "Pillar-0";
	test.dmap = testmap;
	test.pos = start;
	fovtests.push_back(test);
}

void GenerateEmpty()
{
	// PILLAR
	pgn::cArray2D<int> testmap;
	testmap.Resize(40, 40);
	testmap.Fill(pgn::dungen::eMapData::room);
	glm::ivec2 start(20, 20);

	cTestCase test;
	test.name = "Empty";
	test.dmap = testmap;
	test.pos = start;
	fovtests.push_back(test);
}

int main(int argc, char ** argv)
{
	std::string exepath = argv[0];
	exepath = pystring::replace(exepath, "/", "\\");
	std::vector<std::string> chunks;
	pystring::rsplit(exepath, chunks, "\\", 1);
	exepath = chunks[0] + "\\";

	GenerateDiag();
	GeneratePillar();
	GenerateWiggly();
	GenerateEmpty();

	int visBits = pgn::dungen::eMapData::room | pgn::dungen::eMapData::corridor | pgn::dungen::eMapData::conn;

	for (size_t los = 10; los < 11; ++los)
	{
		pgn::ai::cFovLookup<pgn::ai::cFov1> flut;
		auto& fov = flut.Get(los);

		for (const auto& test : fovtests)
		{
			pgn::cArray2D<bool> vismap(test.dmap.Width(), test.dmap.Height());
			pgn::cArray2D<float> visf(test.dmap.Width(), test.dmap.Height(), 0.0f);
			std::vector<glm::ivec2> lospts;
			
			vismap.View().VisitWext([&](size_t x, size_t y, std::vector<bool>::reference v) {v = (test.dmap(x, y) & visBits) ? true : false; });
			fov.Calc(test.pos, vismap, lospts, visf);

			const auto& dmap = test.dmap;

			pgn::cArray2D<SDL_Color> imgData(test.dmap.Width(), test.dmap.Height());
			for (size_t y = 0; y < test.dmap.Height();++y)
			for (size_t x = 0; x < test.dmap.Width(); ++x)
			{
				const auto& elem = test.dmap(x, y);
				glm::ivec3 col(0, 0, 0);
				if (elem & pgn::dungen::eMapData::corridor)
					col += glm::ivec3(255, 255, 0);
				if (elem & pgn::dungen::eMapData::room)
					col += glm::ivec3(0, 255, 255);
				if (elem & pgn::dungen::eMapData::blocked)
					col -= glm::ivec3(16, 16, 16);
				if (elem & pgn::dungen::eMapData::perimeter)
					col += glm::ivec3(64, 64, 64);
				// conn -> green
				if (elem & pgn::dungen::eMapData::conn)
					col += glm::ivec3(255, -255, -255);

				if (elem & (pgn::dungen::eMapData::entry | pgn::dungen::eMapData::exit))
					col = glm::ivec3(0, 0, 255);
				imgData(x, y).r = std::min(std::max(col.x, 0), 127) + Uint8(128*visf(x,y));
				imgData(x, y).g = std::min(std::max(col.y, 0), 255);
				imgData(x, y).b = std::min(std::max(col.z, 0), 255);
				imgData(x, y).a = 255;
			};

			pgn::io::SaveImage(exepath + "fovtest-"+test.name + ".png", imgData);
		}
	}
}