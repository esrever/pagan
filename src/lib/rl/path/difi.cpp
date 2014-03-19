#include "difi.h"

#include <algorithm>
#include <deque>

namespace pgn
{
	namespace rl
	{
		void cDiFi::Init(const index2d_type& dims)
		{
			mMap.Resize(dims.x, dims.y);
			assert(std::max(dims.x, dims.y) < std::numeric_limits<data_type>::max());

			mCornerWcs = glm::ivec2(0, 0);
		}

		//---------------------------------------------------
		void cDiFi::Generate(movecost_func_type f, const glm::ivec2& corner, const glm::ivec2& goal)
		{
			std::vector<glm::ivec2> g; g.push_back(goal);
			Generate(f, corner, g);
		}
		void cDiFi::Generate(movecost_func_type f, const glm::ivec2& corner, const std::vector<glm::ivec2>& goals)
		{
			mCornerWcs = corner;

			// Initialize map to max disrtances
			mMap.Fill(std::numeric_limits<data_type>::max());

			// Initialize todo points: start with the goal points
			std::deque<glm::ivec2> stack_p;
			for (const auto& p : goals)
			{
				stack_p.push_back(p-mCornerWcs);
			}

			// Initialize todo point distances: start with zeros
			std::deque<data_type> stack_v(goals.size(), data_type(0));

			// store the horz/diag offset vectors
			const glm::ivec2 off_hd[] = { glm::ivec2(-1, 0), glm::ivec2(1, 0), glm::ivec2(0, -1), glm::ivec2(0, 1),
										  glm::ivec2(-1, -1), glm::ivec2(1, -1), glm::ivec2(-1, 1), glm::ivec2(1, 1) };

			// While we have stuff to process
			while (!stack_p.empty())
			{
				// pop a point and val
				const auto p = stack_p.front();
				const auto v = stack_v.front();
				stack_p.pop_front();
				stack_v.pop_front();
				// If we're in range
				
				{	
					// if we need to update the stored distance
					if (v < mMap(p))
					{
						// update it
						mMap(p) = v;
						// propagate
						for (int i = 0; i < 8; ++i)
						{
							auto p2 = p + off_hd[i];
							if (mMap.InRange(p2))
							{
								auto vadd = f(p + mCornerWcs, p2+mCornerWcs);
								if (vadd != std::numeric_limits<data_type>::max())
								{
									stack_p.push_back(p2);
									stack_v.push_back(v + vadd);
								}
							}
						}
					}
				}
			}
		}
	}
}

