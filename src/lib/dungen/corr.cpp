#include "corr.h"

#include "dungen.h"

namespace pgn
{
	namespace dungen
	{

		//-----------------------------------------------------
		cCorr::cCorr(cWorkspace& ws, const std::deque<glm::ivec2>& pts)
			:cArea(ws)
		{
			mIsRoom = false;
			for (const auto& x : pts)
				mPts.push_back(x);
		}

		void cCorr::Init()
		{
			// Fill the dense map
			for (auto x : mPts)
			{
				mWorkspace.mMapData(x) |= eMapData::corridor;
			}
			update_perimeter();
			// TODO: fix! 2 is the minimum. 
			mMaxConns = 2;
		}


		//----------------------------------------------------------------------------------------------------------
		void cCorr::update_perimeter()
		{
			// clear existing perimeter
			cArea::update_perimeter();
			// for each point on perimeter
			for (const auto& p : mPts)
			{
				// get 3x3 mask
				for (int i = -1; i <= 1; ++i)
				{
					for (int j = -1; j <= 1; ++j)
					{
						// is it in range?
						glm::ivec2 pt(p.x + j, p.y + i);
						if (!mWorkspace.mMapData.InRange(pt))
							continue;

						// in range, so room/corr are OR'd with perimeter
						auto v = mWorkspace.mMapData(pt);
						if (!(v & (eMapData::room | eMapData::corridor)))
						{
							mWorkspace.mPerimeterMap(pt).push_back(mId);

							// add dense
							mWorkspace.mMapData(pt) |= eMapData::perimeter;
							// add to the perimeter points
							mPerimeter.insert(pt);
						}
					}
				}
			}
			calc_avail_connection_slots();
		}

		//----------------------------------------------------------------------------------------------------------
		void cCorr::calc_avail_connection_slots()
		{
			// reset
			for (const auto& pt : mAvailConnectionSlots)
			{
				bool found = false;
				auto& spi = mWorkspace.mConnectionMap(pt);
				for (int i = 0; i < int(spi.size()); ++i)
				{
					if (spi[i] == mId)
					{
						found = true;
						spi.erase(spi.begin() + i);
						--i;
					}
				}
				assert(found);
			}
			mAvailConnectionSlots.clear();

			std::vector<glm::ivec2> ppt_cross;
			// for each point in the perimeter:
			for (const auto & ppt : mPerimeter)
			{
				bool ok = true;

				auto v = mWorkspace.mMapData(ppt);

				// if it's blocked, break
				if (v & eMapData::blocked)
					ok = false;
				if (!ok)
					continue;

				//# if this point is a corner point, break
				if (mWorkspace.is_perimeter_corner(ppt))
					continue;

				auto iters = ai::cShapeCalc<ai::cDiamondDistance>::Get(1, 1);
				for (auto it = iters.first; it != iters.second; ++it)
				{
					auto x = ppt + *it;
					if (mWorkspace.mMapData.InRange(x) && (mWorkspace.mMapData(x) & eMapData::conn))
						ok = false;
				}
				if (!ok)
					continue;

				mAvailConnectionSlots.push_back(ppt);
			}

			std::random_shuffle(mAvailConnectionSlots.begin(), mAvailConnectionSlots.end());

			// ConnMap
			for (const auto& pt : mAvailConnectionSlots)
			{
				auto& spi = mWorkspace.mConnectionMap(pt);
				spi.push_back(mId);
			}
		}

	}
}