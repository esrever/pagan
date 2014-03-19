#include "constraints.h"

#include <rl/utils/shape/ShapeCalc.h>

#include "dungen.h"
#include "area.h"

namespace pgn
{
	namespace rl
	{

		//----------------------------------------------------------------------------------------------------------
		bool cons_door2door_min_distance_check(const cArea& area, const glm::ivec2& pt)
		{
			bool ok = true;
			// distance check from existing connections from the nb area
			for (const auto & conn : area.mConnections)
			{
				const auto& connpt = conn.first;
				auto d = size_t(norm_inf(pt - connpt));

				auto door2doorMinDistance = area.mWorkspace.randu(area.mWorkspace.mConstraints.mDoorToDoorMinDistance);
				if (d < door2doorMinDistance)
				{
					ok = false;
					break;
				}
			}
			return ok;
		}

		//----------------------------------------------------------------------------------------------------------
		bool is_perimeter_corner(const cArea& area, const glm::ivec2& pt)
		{
			size_t horz_nbs_num = 0, vert_nbs_num = 0;
			auto iters = rl::cShapeCalc<rl::cDiamondDistance>::Get(1, 1);
			for (auto it = iters.first; it != iters.second; ++it)
			{
				auto x = pt + *it;
				if (!area.mWorkspace.mMapData.InRange(x)) continue;
				if (area.mPerimeter.find(x) == area.mPerimeter.end()) continue;
				auto d = x - pt;
				if (d.x)
					horz_nbs_num++;
				if (d.y)
					vert_nbs_num++;
			}
			assert(horz_nbs_num | vert_nbs_num);
			return (horz_nbs_num * vert_nbs_num) != 0;
		}
	}
}