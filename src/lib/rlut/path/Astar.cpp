#include "Astar.h"

#include <core/math/norm.h>
#include <core/container/Array2D_utils.h>

namespace pgn
{
	namespace rlut
	{
		

		void cAstar::ReconstructPath(const glm::ivec2& start, const glm::ivec2& goal, std::deque<glm::ivec2>& path, const glm::ivec2& offset)
		{
			path.clear();
			path.push_front(goal + offset);
			path.push_front(mLinks(goal) + offset);
			glm::ivec2 start1 = start + offset;
			while (path.front() != start1)
				path.push_front(mLinks(path.front() - offset) + offset);
		}
	}
}