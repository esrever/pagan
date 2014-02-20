#include <pystring.h>
#include <SDL.h>
#undef main
#include <SDL_image.h>

#include <core/util/color.h>
#include <io/image/utils.h>

int main(int argc, char ** argv)
{
	/*
		What's useful for me?
			Palette image:
				A list of colours
				data are indices to list of colours
				toy interface:
					cArray2D<char>    palette index data
					vector<SDL_Color> palette colours
			RGB image:
				cArray<SDL_Color> colour data
	*/


	std::string exepath = argv[0];
	exepath = pystring::replace(exepath, "/", "\\");
	std::vector<std::string> chunks;
	pystring::rsplit(exepath, chunks, "\\", 1);
	exepath = chunks[0] + "\\";


	const auto outfname = exepath + "io-test1.png";

	pgn::cArray2D<char> cidxMap;
	cidxMap.Resize(64, 32, 0);
	std::vector<SDL_Color> colorMap;
	SDL_Color c;
	c.r = 255; c.g = 0; c.b = 0; c.a = 255; colorMap.push_back(c);
	c.r = 0; c.g = 255; c.b = 0; c.a = 255; colorMap.push_back(c);
	c.r = 0; c.g = 0; c.b = 255; c.a = 255; colorMap.push_back(c);
	cidxMap.View().VisitWext([](size_t x, size_t y, char& c){ c = (x&1) + (y&1); });

	pgn::io::SavePaletteImage(outfname, cidxMap, colorMap);


	pgn::cArray2D<char> gradMap;
	gradMap.Resize(256, 1, 0);
	std::vector<SDL_Color> gradColorMap;
	pgn::GenGrayscaleGradient(gradColorMap);
	gradMap.View().VisitWext([](size_t x, size_t y, char& c){ c = x; });

	const auto outfname2 = exepath + "io-test1-grad.png";
	pgn::io::SavePaletteImage(outfname2, gradMap, gradColorMap);
	return 0;
}