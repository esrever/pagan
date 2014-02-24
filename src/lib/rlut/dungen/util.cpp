#include "util.h"

#include <rlut/path/Astar.h>
#include <rlut/path/Astar.cpp>

#include "dungen.h"
#include "area.h"

namespace pgn
{
	namespace rlut
	{
		cAnyGoalFirst::cAnyGoalFirst(const cWorkspace& ws, const glm::ivec2& start, size_t thisId, const std::set<size_t> excludeIds)
			:mWs(ws)
			, mStart(start)
			, mStartId(thisId)
			, mExcludeIds(excludeIds){}

		bool cAnyGoalFirst::IsGoal(const glm::ivec2& pt) const
		{
			const auto& spi = mWs.mConnectionMap(pt.x, pt.y);
			if (!spi.empty())
			{
				size_t spi_found = 0xFFFFFFFF;
				if (mStart == pt)
				{
					// allow only if we have at least one more conn area that is not this or in the excluded list
					if (spi.size() == 2)
					{
						spi_found = spi[0] == mStartId ? spi[1] : spi[0];
					}
				}
				else
				{
					// allow only if we have only one conn area that is not this or in the excluded list
					if ((spi.size() == 1) && (spi[0] != mStartId))
						spi_found = spi[0];
				}
				if (spi_found < mWs.mAreas.size())
				{
					const auto& area = mWs.mAreas[spi_found];
					bool can_connect = (area->mConnections.size() < area->mMaxConns) | mWs.mConstraints.mOverrideMaxConns;
					if (can_connect)
					{
						//std::cout << "start: " << mWorkspace.mAreas[startId - 1]->mConnections.size() << " " <<mWorkspace.mAreas[startId - 1]->mMaxConns << std::endl;
						//std::cout << "end: " << spi_found->mConnections.size() << " " << spi_found->mMaxConns << std::endl;
						bool not_excl = mExcludeIds.find(spi_found) == mExcludeIds.end();
						return not_excl;
					}
				}

			}
			return false;
		}

		//----------------------------------------------------------------------------------------------------------------
		std::function<float(const glm::ivec2&, const glm::ivec2&)> GenerateMoveCostFunction(const cWorkspace& ws, int invBits, int reqBits)
		{
			return [&ws, invBits, reqBits](const glm::ivec2&, const glm::ivec2& p){ return ws.MoveCost(p, invBits, reqBits); };
		}
	}
	
	namespace rlut
	{
		template bool cAstar::Search<rlut::cAnyGoalFirst>(const glm::ivec2& start0,
			const rlut::cAnyGoalFirst& goal0,
			std::deque<glm::ivec2>& path,
			const cArrayShape2D& searchSpace,
			movecost_func_type gfunc,
			const cShapeCalcTraits::ret_type& nbs,
			float alpha);
	}
}
