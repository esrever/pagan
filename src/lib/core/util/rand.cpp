#include "rand.h"
#include <boost/random.hpp>

namespace pgn
{
	float randf()
	{
		return randf(0.0f, 1.0f);
	}

	float randf(float vmin, float vmax)
	{
		return 0.0f;
	}

	size_t randu(size_t vmin, size_t vmax)
	{
		return 0;
	}
}