#include "room.h"

#include <algorithm>
#include <bitset>

#include "dungen.h"
#include "corr.h"

namespace pgn
{
	namespace rl
	{

		//-----------------------------------------------------
		cRoom::cRoom(cWorkspace& ws, const glm::ivec2& start, const glm::ivec2& dims)
			:cArea(ws)
			, mStart(start)
			, mDims(dims)
		{
			mIsRoom = true;
		}

		void cRoom::Init()
		{
			// Fill the dense map
			mWorkspace.mMapData.CreateView(mDims.x, mDims.y, mStart.x, mStart.y).Fill(eMapData::room);

			update_perimeter();
			calc_avail_connection_slots();
		}

		int cRoom::which_side_mask(const glm::ivec2& ppt) const
		{
			int ret = 0;
			if (ppt.x == (mStart.x - 1))		 ret |= eRoomSideMask::left; // left
			if (ppt.x == (mStart.x + mDims.x)) ret |= eRoomSideMask::right; // right
			if (ppt.y == (mStart.y - 1))		 ret |= eRoomSideMask::bottom; // bot
			if (ppt.y == (mStart.y + mDims.y)) ret |= eRoomSideMask::top; // top
			assert(ret > 0);
			return ret;
		}

		const int eRoomSideMask::val2idx(int v)
		{
			switch (v)
			{
			case 1: return 0;
			case 2: return 1;
			case 4: return 2;
			case 8: return 3;
			default:
				return -1;
			};
		}

		int cRoom::which_side_mask_idx(const glm::ivec2& ppt) const
		{
			int m = which_side_mask(ppt);
			return eRoomSideMask::val2idx(m);
		}

		glm::ivec2 cRoom::side_midpoint(int side) const
		{
			glm::ivec2 ret = mStart;
			glm::ivec2 hdims = mDims / int(2);
			switch (side)
			{
			case eRoomSideMask::bottom: ret.x += hdims.x; break;
			case eRoomSideMask::top: ret.x += hdims.x; ret.y += mDims.y; break;
			case eRoomSideMask::left: ret.y += hdims.y; break;
			case eRoomSideMask::right: ret.x += mDims.x; ret.y += hdims.y; break;
			default:
				break;
			}
			return ret;
		}

		//-----------------------------------------------------
		cRoom::ptr_type cRoom::create(cWorkspace& ws)
		{
			// generate a starting point
			glm::ivec2 start;
			glm::ivec2 map_range_x(0, ws.mMapData.Width());
			glm::ivec2 map_range_y(0, ws.mMapData.Height());
			start.x = ws.randu(map_range_x);
			start.y = ws.randu(map_range_y);

			// run creator
			return create(ws, start);
		}

		//-----------------------------------------------------
		cRoom::ptr_type cRoom::create(cWorkspace& ws, const glm::ivec2& start)
		{
			// generate the rect
			glm::ivec2 dims;
			dims.x = ws.randu(ws.mConstraints.mRoomRectW);
			dims.y = ws.randu(ws.mConstraints.mRoomRectH);

			//# is it within bounds?
			auto start_perim = start - glm::ivec2(1, 1);
			auto end_perim = start + dims + glm::ivec2(1, 1);
			if (!(ws.mMapData.InRange(start_perim) && ws.mMapData.InRange(end_perim)))
				return nullptr;

			// get room rect
			auto sub_perim = ws.mMapData.CreateView(dims.x + 2, dims.y + 2,start.x - 1, start.y - 1);
			auto sub = ws.mMapData.CreateView(dims.x, dims.y,start.x, start.y);

			bool used = false;
			cArrayView< cDenseStorage<int>>::visit_read_func f = [&used](const int& v){ if (v) used = true; };
			sub.VisitR(f);
			if(used)
				return nullptr;

			//# are perimeter corners unused or non-conns? TODO: more generic? perimeter_corners function?
			glm::ivec2 corners[] = { glm::ivec2(start_perim.x, start_perim.y),
				glm::ivec2(end_perim.x, start_perim.y),
				glm::ivec2(start_perim.x, end_perim.y),
				glm::ivec2(end_perim.x, end_perim.y) };
			for (size_t i = 0; i < 4; ++i)
			{
				auto v = ws.mMapData(corners[i]);

				if (v & eMapData::conn)
					return nullptr;
			}

			// all ok, create the room
			auto room = std::make_shared<cRoom>(cRoom(ws, start, dims));
			ws.mAreas.push_back(room);
			return room;
		}

		//-----------------------------------------------------
		void cRoom::update_perimeter()
		{
			cArea::update_perimeter();

			// Apply 'room' in map
			mWorkspace.mMapData.CreateView(mDims.x, mDims.y, mStart.x, mStart.y).VisitW([](int& i){ i |= eMapData::room; });
			auto f = [](int& i){ i |= eMapData::perimeter; };

			// Apply 'perimeter' in map
			auto x = mStart.x - 1;
			auto y = mStart.y - 1;
			auto w = mDims.x + 2;
			auto h = mDims.y + 2;
			mWorkspace.mMapData.CreateView(w,1,x,y).VisitW(f);
			mWorkspace.mMapData.CreateView(w,1, x, y+h-1).VisitW(f);
			mWorkspace.mMapData.CreateView(1, h, x, y).VisitW(f);
			mWorkspace.mMapData.CreateView(1, h, x+w-1, y).VisitW(f);

			// add perimeter points
			// vertical
			auto area = mWorkspace.mAreas[mId];
			for (int i = 0; i < h; ++i)
			{
				auto pt0 = glm::ivec2(x, y + i);
				auto pt1 = glm::ivec2(x + w - 1, y + i);
				mWorkspace.mPerimeterMap(pt0).push_back(area->mId);
				mWorkspace.mPerimeterMap(pt1).push_back(area->mId);
				mPerimeter.insert(pt0);
				mPerimeter.insert(pt1);
			}
			// horizontal
			for (int j = 0; j < w; ++j)
			{
				auto pt0 = glm::ivec2(x + j, y);
				auto pt1 = glm::ivec2(x + j, y + h - 1);
				mWorkspace.mPerimeterMap(pt0).push_back(area->mId);
				mWorkspace.mPerimeterMap(pt1).push_back(area->mId);
				mPerimeter.insert(pt0);
				mPerimeter.insert(pt1);
			}

			// calculate which connections are free
			calc_avail_connection_slots();
		}

		//-----------------------------------------------------
		void cRoom::calc_avail_connection_slots()
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


			// for each point in the perimeter:
			std::vector<glm::ivec2> ppt_cross;
			ppt_cross.reserve(4);
			for (const auto & ppt : mPerimeter)
			{
				bool ok = true;

				//# Get side and check if there's space on the other side ( unused or corr or conn)
				auto side_mask = which_side_mask(ppt);
				glm::ivec2 ppt_nb;
				switch (which_side_mask(ppt))
				{
				case eRoomSideMask::left:
					ppt_nb = glm::ivec2(ppt.x - 1, ppt.y);
					break;
				case eRoomSideMask::right:
					ppt_nb = glm::ivec2(ppt.x + 1, ppt.y);
					break;
				case eRoomSideMask::bottom:
					ppt_nb = glm::ivec2(ppt.x, ppt.y - 1);
					break;
				case eRoomSideMask::top:
					ppt_nb = glm::ivec2(ppt.x, ppt.y + 1);
					break;
				default:
					ok = false;
					break;
				}

				if (!ok)
					continue;

				// if not in map range, break
				if (!mWorkspace.mMapData.InRange(ppt_nb))
					ok = false;
				if (!ok)
					continue;

				//# if we got a neighbour, check if it's unused or corr or conn or room
				int vcc = eMapData::corridor | eMapData::conn | eMapData::room;
				auto val = mWorkspace.mMapData(ppt_nb);
				ok = (val == 0) || (val & vcc);

				if (!ok)
					continue;

				//# if this point is a corner point, break
				if (mWorkspace.is_perimeter_corner(ppt))
					continue;

				auto iters = rl::cShapeCalc<rl::cDiamondDistance>::Get(1, mWorkspace.randu(mWorkspace.mConstraints.mDoorToDoorMinDistance));
				for (auto it = iters.first; it != iters.second; ++it)
				{
					auto x = ppt + *it;
					if (mWorkspace.mMapData.InRange(x) && (mWorkspace.mMapData(x) & eMapData::conn))
						ok = false;
				}

				if (!ok)
					continue;

				//# distance check from existing connections from this area
				ok = cons_door2door_min_distance_check(*this, ppt);
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