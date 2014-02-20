#include "color.h"

namespace pgn
{
	void GenGrayscaleGradient(std::vector<SDL_Color>& grad, Uint8 start, Uint8 end, size_t num)
	{
		grad.resize(num);
		double gstep = end - start / (num - 1.0);
		for (size_t i = 0; i < num;++i)
		{
			auto& c = grad[i];
			c.r = c.g = c.b = Uint8(std::round(start + (end - start)*gstep*i));
			c.a = 255;
		}
	}
}